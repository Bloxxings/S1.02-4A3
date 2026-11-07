#include <algorithm>
#include <cctype>
#include <iostream>
#include <limits>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct Joueur {
    string nom;
    int points = 0;
    int victoires = 0;
};

struct Action {
    string nomJoueur;
    int choix;
};

string nettoyer(string texte) {
    size_t debut = texte.find_first_not_of(" \t\r\n");
    if (debut == string::npos) return "";
    size_t fin = texte.find_last_not_of(" \t\r\n");
    return texte.substr(debut, fin - debut + 1);
}

string enMinuscules(string texte) {
    transform(texte.begin(), texte.end(), texte.begin(),
              [](unsigned char c) { return static_cast<char>(tolower(c)); });
    return texte;
}

bool commencePar(const string& texte, const string& prefixe) {
    return texte.compare(0, prefixe.size(), prefixe) == 0;
}

// Lit un nombre entier et refuse les caractères en trop.
bool lireEntier(const string& texte, int& valeur) {
    istringstream flux(texte);
    string reste;
    return (flux >> valeur) && !(flux >> reste);
}

int convertirChoix(const string& texte) {
    int valeur;
    if (!lireEntier(texte, valeur) || valeur < 1 || valeur > 6) return -1;
    return valeur;
}

int main() {
    const int nombreDeRondes = 3;
    vector<Joueur> joueurs;
    vector<Action> actions;


    string ligne;
    while (getline(cin, ligne)) {
        ligne = nettoyer(ligne);
        if (ligne.empty() || ligne[0] == '#') continue;

        if (commencePar(ligne, "Joueur:")) {
            string nom = nettoyer(ligne.substr(7));
            if (nom.empty()) {
                cerr << "Erreur : le nom d'un joueur est vide.\n";
                return 1;
            }
            joueurs.push_back({nom, 0, 0});
        } else if (commencePar(ligne, "Action de ")) {
            size_t deuxPoints = ligne.find(':', 10);
            if (deuxPoints == string::npos) {
                cerr << "Erreur : format attendu : Action de Nom: choix.\n";
                return 1;
            }

            string nom = nettoyer(ligne.substr(10, deuxPoints - 10));
            string texteChoix = nettoyer(ligne.substr(deuxPoints + 1));
            int choix = convertirChoix(texteChoix);
            if (nom.empty() || choix == -1) {
                cerr << "Erreur : action invalide. Vérifie le nom et le choix.\n";
                return 1;
            }
            actions.push_back({nom, choix});

        } else {
            cerr << "Erreur : ligne inconnue : " << ligne << '\n';
            return 1;
        }
    }

    if (joueurs.size() < 2 || joueurs.size() % 2 != 0) {
        cerr << "Erreur : il faut un nombre pair de joueurs, au moins deux.\n";
        return 1;
    }

    set<string> nomsUniques;
    for (const Joueur& joueur : joueurs) {
        if (!nomsUniques.insert(joueur.nom).second) {
            cerr << "Erreur : deux joueurs ont le même nom : " << joueur.nom << ".\n";
            return 1;
        }
    }

    vector<vector<bool>> dejaJoue(joueurs.size(), vector<bool>(joueurs.size(), false));
    size_t prochaineAction = 0;


    for (int ronde = 1; ronde <= nombreDeRondes; ronde++) {
        vector<int> ordre;
        for (int i = 0; i < static_cast<int>(joueurs.size()); i++) ordre.push_back(i);

        // Le joueur qui a le plus de points est placé en premier.
        // En cas d'égalité, l'ordre d'inscription est conservé.
        stable_sort(ordre.begin(), ordre.end(), [&](int a, int b) {
            return joueurs[a].points > joueurs[b].points;
        });

        vector<bool> dejaApparie(joueurs.size(), false);
        for (int joueur1 : ordre) {
            if (dejaApparie[joueur1]) continue;

            int joueur2 = -1;
            int meilleurEcart = numeric_limits<int>::max();

            // Cherche le joueur disponible au score le plus proche,
            // en évitant une revanche si c'est possible.
            for (int candidat : ordre) {
                if (candidat == joueur1 || dejaApparie[candidat] || dejaJoue[joueur1][candidat]) continue;
                int ecart = abs(joueurs[joueur1].points - joueurs[candidat].points);
                if (ecart < meilleurEcart) {
                    meilleurEcart = ecart;
                    joueur2 = candidat;
                }
            }

            // Si tout le monde a déjà joué ensemble, une revanche est permise.
            if (joueur2 == -1) {
                for (int candidat : ordre) {
                    if (candidat != joueur1 && !dejaApparie[candidat]) {
                        joueur2 = candidat;
                        break;
                    }
                }
            }

            if (joueur2 == -1) {
                cerr << "Erreur : impossible de former un match.\n";
                return 1;
            }
            dejaApparie[joueur1] = true;
            dejaApparie[joueur2] = true;

            if (prochaineAction + 1 >= actions.size()) {
                cerr << "Erreur : il manque des actions dans le fichier d'entrée.\n";
                return 1;
            }
            const Action& action1 = actions[prochaineAction++];
            const Action& action2 = actions[prochaineAction++];

            if (action1.nomJoueur != joueurs[joueur1].nom || action2.nomJoueur != joueurs[joueur2].nom) {
                cerr << "Erreur : les actions ne correspondent pas au match attendu entre "
                     << joueurs[joueur1].nom << " et " << joueurs[joueur2].nom << ".\n";
                return 1;
            }

            // Au dé, le plus grand résultat gagne. Une égalité donne un point à chacun.
            if (action1.choix > action2.choix) {
                joueurs[joueur1].points += 3;
                joueurs[joueur1].victoires++;
            } else if (action2.choix > action1.choix) {
                joueurs[joueur2].points += 3;
                joueurs[joueur2].victoires++;
            } else {
                joueurs[joueur1].points++;
                joueurs[joueur2].points++;
            }

            dejaJoue[joueur1][joueur2] = true;
            dejaJoue[joueur2][joueur1] = true;
        }
    }

    if (prochaineAction != actions.size()) {
        cerr << "Erreur : il y a trop d'actions dans le fichier d'entrée.\n";
        return 1;
    }


    vector<int> classement;
    for (int i = 0; i < static_cast<int>(joueurs.size()); i++) classement.push_back(i);
    stable_sort(classement.begin(), classement.end(), [&](int a, int b) {
        if (joueurs[a].points != joueurs[b].points) return joueurs[a].points > joueurs[b].points;
        return joueurs[a].victoires > joueurs[b].victoires;
    });

    // La sortie standard ne contient que le classement attendu par diff.
    for (size_t i = 0; i < classement.size(); i++) {
        cout << i + 1 << ':' << joueurs[classement[i]].nom << '\n';
    }

    // Les détails sont envoyés sur stderr, séparé du résultat comparé à l'oracle.
    cerr << "Scores calcules :\n";
    for (const Joueur& joueur : joueurs) {
        cerr << joueur.nom << " : " << joueur.points << " points, "
             << joueur.victoires << " victoires\n";
    }
    return 0;
}
