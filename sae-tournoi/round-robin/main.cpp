#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

struct joueur {
    string nom;
    int choix;
    int points;
};

// Lit une ligne en sautant les commentaires '#'
string litLigne() {
    // à écrire
}

void litJoueurs(vector<joueur> & joueurs)
{
    unsigned int nombre_joueurs = 0;
    cerr << "Entrez le nombre de joueurs" << endl;
    cin >> nombre_joueurs;
    joueurs.resize(nombre_joueurs);
    for (unsigned int i = 0; i < nombre_joueurs; i = i+1)
    {
        cerr << "Entrez le prénom du "<< i+1 << " joueur" << endl;
        cin >> joueurs[i].nom;
        cerr << "Entrez la réponse du "<< i+1 << " joueur" << endl;
        cin >> joueurs[i].choix;
        while (joueurs[i].choix < 1 || joueurs[i].choix > 3)
        {
            cerr << "Veuillez choisir un chiffre entre 1 et 3 "<< endl;
            cin >> joueurs[i].choix;
        }
    }
}

// Renvoie 1 si a gagne, 2 si b gagne, 0 si égalité
int resultatMatch(int a, int b) {
    // à écrire
}

// Fait jouer chaque paire une fois et ajoute les points
void joueTournoi(vector<joueur> & joueurs) {
    // à écrire
}

// Compare deux joueurs pour le tri (points, puis départage)
bool compareJoueurs(const joueur & a, const joueur & b) {
    // à écrire
}

// Affiche "rang:nom" pour chaque joueur
void affiche(const vector<joueur> & joueurs) {
    // à écrire
}

int main()
{
    vector<joueur> joueurs;
    litJoueurs(joueurs);
    if (joueurs.size() < 2) {
        return 1;
    }
    joueTournoi(joueurs);
    stable_sort(joueurs.begin(), joueurs.end(), compareJoueurs);
    affiche(joueurs);
    return 0;
}