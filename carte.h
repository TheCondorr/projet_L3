class carte {
    private: 
        char color;                 //Couleur de la carte
        int n;                  //Hauteur de la carte (Valet = 11, Dame = 12, Roi = 13)
        int v;                  //Valeur de la carte au blackjack (As = 1 ou 11, 2-10 = 2-10, tetes = 10)
    public:
        carte(char c, int n, int v);
        carte();
        ~carte();
        char GetColor();
        int GetN();
        int GetV();
        void affiche();
        void affiche_value();
        bool assert_value();
};