#include "carte.h"
#include <string>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <map>
#include <time.h>

using namespace std;

int give_value(int n) {               //Renvoie la valeur d'une carte au blackjack
    if (n==11 || n==12 || n==13) {      //Les buches ont une valeur de 10
        return 10;
    }
    else {                              //On initialise tous les As à 1 point.
        return n;
    }
};

int indice_maximum_vect_int(vector<int> vecteur) {
    int maximum = vecteur[0];
    int i = 0;
    for (int k=0; k<vecteur.size(); k++) {
        if (vecteur[k] > maximum) {
            maximum = vecteur[k];
            i = k;
        }
    }
    return i;
}

bool check_deck(vector<carte> deck) {           //Verification de la validite du deck
    for (int i=0; i<52; i++) {                 
        if (!(deck[i].assert_value())) {
            return false;
        };                
    }
    return true;
};

vector<carte> shuffle_deck(vector<carte> base_deck) {
    vector<carte> deck (52);
    int replaced = 0;
    int card = 0;
    int nb = 0;
    srand(time(NULL));
    while (replaced<52) {
        int i = rand() % 52;
        int v = deck[i].GetV();
        if (v == 0) {
            deck[i] = base_deck[card];
            card ++;
            replaced ++;
        }
    }  
    return deck;
};

vector<carte> create_deck() {
    vector <carte> deck (52);
    char color;
    int number;
    int value;
    for (int i=0; i<52; i++) {                  //Creation d'un deck
        if (i<13) {                             //Separation des couleurs en fonctions de i
            color = 'h';
        }
        else if (i<26) {
            color = 'd';
        }
        else if (i<39) {
            color = 'c';
        }
        else {
            color = 's';
        }
        if ((i+1)%13 == 0) {                    //Ajustement de i par rapport aux numeros des cartes
            number = 13;
        }
        else {
            number = (i+1)%13;                  
        }
        value = give_value(number);             //Affectation de la bonne valeur pour une carte donnee
        deck[i] = carte(color,number,value);
        if (!(check_deck(deck))) {                //Assure que le deck au complet est bien valide
            break;
        } 
    }
    return deck;
};

void affiche_deck(vector<carte> deck, int size) {
    for (int i=0; i<size; i++) {
        deck[i].affiche();
    }
};

bool piocher(int joueur, vector<carte> visibles, map<int,vector<carte>> tours) {
    map<int, vector<carte>>::iterator it = tours.begin();
    carte visible = visibles[joueur];
    vector<int> points(2);
    if (visible.GetN() != 1) {
        points[0] = visible.GetV();
        points[1] = visible.GetV();
    } 
    else {
        points[0] = 1;
        points[1] = 11;
    }
    while (it!=tours.end()) {
        vector<carte> cartes_tour = it->second;
        carte card = cartes_tour[joueur];
        int numero = card.GetN();
        if (card.GetN() != 1) {
            points[0] += card.GetV();
            points[1] += card.GetV();
        } 
        else {
            if (points[0] != points[1]) {                   //Si les deux valeurs sont différentes, un as a déjà été tiré par le joueur. Il n'y a alors aucun intérêt à donner la valeur de 11 à 2 As, cela dépasserait automatiquement 21.
                points[0] += 1;
                points[1] += 1;
            }
            else {
                points[0] += 1;
                points[1] += 11;
            }
        }
        it++;
    }
    int valeur_main = 22;
    int max = 21;
    while (valeur_main == 22 & max>0) {
        for (int k=0; k<2; k++) {
            if (points[k] == max) {
                valeur_main = max;                          //On choisit ici de toujours considérer la valeur maximale inférieure à 21) pour décider de tirer ou non.
            }
        }
        max--;
    }
    if (valeur_main<17) {
        return true;
    } 
    else {return false;}   
}

vector<carte> make_visibles(vector<carte> deck, int nb_j) {
    vector<carte> visibles(nb_j);                   //Cartes visibles par tous
    for (int i=0; i<nb_j; i++) {
        visibles[i] = deck[i];
    }
    return visibles;
}

map<int,vector<carte>> jeu(vector<carte> deck, vector<carte> visibles, int nb_j) {
    vector<carte> perso(nb_j);                      //Premiere carte cachee de chaque joueur
    int index_carte = nb_j;
    for (int i=0; i<nb_j; i++) {
        perso[i] = deck[index_carte+i];
    }
    index_carte += nb_j;
    map <int, vector<carte>> tours;                         //Dictionnaire ayant pour clés les nombre de tours, comme valeur un tableau de cartes, dont chaque élément est une carte piochee par le joueur i(index du tableau) au tour indiqué par la clé
    tours.insert({0, perso});
    int turn = 1;
    while (true) {
        bool doit_jouer = false;
        for (int i=0; i<nb_j; i++) {
            bool action = piocher(i, visibles, tours);
            if (action) {
                doit_jouer = true;
            }
        }
        if (doit_jouer == false) {break;}
        tours.insert({turn, vector<carte>(nb_j, carte('h',0,0))});          //On insert un tableau de cartes nulles et on demande à chaque joueur s'il souhaite piocher
        for (int i=0; i<nb_j; i++) {
            bool action = piocher(i, visibles, tours);
            if (action) {
                carte piochee = deck[index_carte];
                index_carte++;
                tours[turn][i] = piochee;
            }
        }
        turn++;
    }
    cout<<"---------------"<<endl;
    affiche_deck(visibles,nb_j);
    map<int, vector<carte>>::iterator i = tours.begin();
    while (i!=tours.end()) {
        cout<<"---------Nouveau tour----------"<<endl;
        vector<carte> tour_actuel = i->second;
        affiche_deck(tour_actuel,nb_j);
        i++;
    }
    return tours;
};

vector<int> winner(map<int, vector<carte>> jeux, vector<carte> visibles, int nb_j) {
    vector<vector<int>> totaux(nb_j);
    for(int i=0; i<nb_j; i++) {
        vector<int> total_i(2);
        totaux[i] = total_i;
        int numero_v = visibles[i].GetN();
        if (numero_v !=1 ) {
            totaux[i][0] = visibles[i].GetV();
            totaux[i][1] = visibles[i].GetV();
        }
        else {
            if (totaux[i][0] == totaux[i][1]) {
                totaux[i][1] += 11;
            }
            else {
                totaux[i][1] += 1;                  //Si les deux valeurs sont différentes, un as a déjà été tiré par le joueur. Il n'y a alors aucun intérêt à donner la valeur de 11 à 2 As, cela dépasserait automatiquement 21.
            }
            totaux[i][0] += 1;
        }        
    }
    map<int,vector<carte>>::iterator it = jeux.begin();
    while (it!=jeux.end()) {
        vector<carte> tour = it->second;
        for (int k=0; k<nb_j; k++) {
            carte card = tour[k];
            int numero = card.GetN();
            if (numero!=1) {
                for (int j=0; j<2; j++) {
                    totaux[k][j] += tour[k].GetV();
                }
            }
            else {
                if (totaux[k][0] == totaux[k][1]) {
                    totaux[k][1] += 11;
                }
                else {
                    totaux[k][1] += 1;                  //Si les deux valeurs sont différentes, un as a déjà été tiré par le joueur. Il n'y a alors aucun intérêt à donner la valeur de 11 à 2 As, cela dépasserait automatiquement 21.
                }
                totaux[k][0] += 1;
            }
            }
        it++;
    }
    vector<int> gagnants(nb_j, 0);
    int max = 21;
    while (max>0) {
        for (int k=0; k<nb_j; k++) {
            for (int j=0; j<2; j++) {
                if(totaux[k][j] == max && gagnants[k] == 0) {
                    gagnants[k] = max;
                }
            }
        }
    max--;
    }
    return gagnants;
}

int main() {
    vector<carte> base_deck = create_deck();
    vector<carte> deck = shuffle_deck(base_deck);
    cout<<check_deck(deck)<<endl;
    affiche_deck(deck, 52);
    int nb_j = 2;
    vector<carte> visibles = make_visibles(deck, nb_j);
    map<int,vector<carte>> jeux = jeu(deck, visibles, nb_j);
    vector<int> gagnants = winner(jeux, visibles, nb_j);
    for (int k=0; k<nb_j; k++) {
        cout<<gagnants[k]<<endl;
    }
    int indice_gagnant = indice_maximum_vect_int(gagnants);
    if (indice_gagnant==0 && gagnants[0] != 0) {
        cout<<"Le croupier gagne avec un score de "<<gagnants[indice_gagnant]<<endl;
    }
    else if (gagnants[indice_gagnant] != 0) {
        cout<<"Le joueur "<<indice_gagnant<<" gagne avec un score de "<<gagnants[indice_gagnant]<<endl;
    }
    else {
        cout<<"Tout le monde a dépassé 21.";
    }
    return 0;
}