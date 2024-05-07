#include "carte.h"
#include <string>
#include <iostream>

using namespace std;

carte::carte(char couleur, int number, int value) {
    cout<<"appel constructeur "<<couleur<<" "<<number<<" "<<value<<endl;
    color = couleur;
    n = number;
    v = value;
};

// carte::carte() {
//    cout<<"appel constructeur par défaut"<<endl;
//}

//carte::~carte() {
//   cout<<"appel destructeur"<<endl;
//};

carte::carte() {};
carte::~carte() {};

char carte::GetColor() {return color;};
int carte::GetN() {return n;};
int carte::GetV() {return v;};

void carte::affiche() {             //Affichage de la hauteur et de la couleur de la carte
    string hauteur;
    string couleur;
    if (n==1) {
        hauteur = "As";
    }
    else if (n==11) {
        hauteur = "Valet";
    }
    else if (n==12) {
        hauteur = "Reine";
    }
    else if (n==13) {
        hauteur = "Roi";
    }
    else {
        hauteur = to_string(n);
    }
    if (color=='h') {
        couleur = "coeur";
    }
    else if (color=='d') {
        couleur = "carreau";
    }
    else if (color=='c') {
        couleur = "trèfle";
    }
    else if (color=='s') {
        couleur = "pique";
    }
    cout<<hauteur<<" de "<<couleur<<endl;
};

void carte::affiche_value() {
    cout<<"valeur : "<<v<<endl;
}

bool carte::assert_value() {                    //Verifie que la carte a bien la valeur definie selon le blackjack
    if (n==1) {
        return (v==1 || v==11);                 //Deux valeurs possibles pour l'As
    }
    else if (n==11 || n==12 || n==13) {         //Valeur de 10 pour les buches
        return (v==10);
    }
    else {
        return v==n;
    }
};