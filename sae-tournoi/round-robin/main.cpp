#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct participant {
    string prenom;
    int choix = 0;
    int points = 0;
};

int main()
{
    vector<participant> tab; //crée la liste de participant
    int n = 0;

    while (n < 2)
    {
        cerr << "Nombre de participants (2 minimum) : " << endl;
        cin >> n;
        while (!cin) //tant que l'entrée du cin est différente de ce qui est attendu
        {
            if (cin.eof()) return 1;
            cerr << "Veuillez entrer un nombre valide (2 minimum)" << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            cin >> n;
        }
    }

    tab.resize(n); //on resize le tableau aux nombre de participants
    for (int i = 0; i < n; i = i+1)
    {
        // Prénom
        cerr << "Prenom du participant " << i+1 << " : " << endl;
        cin >> tab[i].prenom;

        // Choix
        while (tab[i].choix < 1 || tab[i].choix > 3)
        {
            cerr << "Choix (1 pierre, 2 feuille, 3 ciseaux) : " << endl;
            cin >> tab[i].choix;
            while (!cin)
            {
                if (cin.eof()) return 1;
                cerr << "Veuillez entrer un chiffre valide (1 pierre, 2 feuille, 3 ciseaux)" << endl;
                cin.clear();
                cin.ignore(10000, '\n');
                cin >> tab[i].choix;
            }
        }
    }

    for (int i = 0; i < n; i = i+1)
        cout << tab[i].prenom << " " << tab[i].choix << endl;

    return 0;
}