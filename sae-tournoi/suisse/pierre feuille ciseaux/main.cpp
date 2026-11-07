#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

// Les informations qu'on conserve pour chaque joueur.
struct Joueur {
    string nom;
    int points = 0;
    int victoires = 0;
};

// Une action garde le nom du joueur et son choix.
struct Action {
    string nomJoueur;
    int choix;
};

// Enlever les espaces inutiles autour d'un texte.
string nettoyer(string texte) {
    size_t debut = texte.find_first_not_of(" \t\r\n");
    if (debut == string::npos) {
        return "";
    }
    size_t fin = texte.find_last_not_of(" \t\r\n");
    return texte.substr(debut, fin - debut + 1);
}

// Convertir un mot en minuscules pour accepter Pierre, PIERRE ou pierre.
string enMinuscules(string texte) {
    transform(texte.begin(), texte.end(), texte.begin(),
              [](unsigned char caractere) {
                  return static_cast<char>(tolower(caractere));
              });
    return texte;
}

// Compare deux choix de pierre-feuille-ciseaux.
// Résultat : 1 si le joueur 1 gagne,
//           -1 si le joueur 2 gagne,
//            0 si égalité.
int resultatDuMatch(int choix1, int choix2) {
    if (choix1 == choix2) {
        return 0;
    }

    bool joueur1Gagne =
        (choix1 == 0 && choix2 == 2) || // pierre bat ciseaux
        (choix1 == 1 && choix2 == 0) || // feuille bat pierre
        (choix1 == 2 && choix2 == 1);   // ciseaux bat feuille

    if (joueur1Gagne) {
        return 1;
    }

    return -1;
}

int main() {
    const int nombreDeRondes = 3;

    // ÉTAPE 1 : lire les joueurs et les actions.
    vector<Joueur> joueurs;
    vector<Action> actions;
    string ligne;

    while (getline(cin, ligne)) {
        // Ignorer les lignes vides et les commentaires.
        if (ligne.empty() || ligne[0] == '#') {
            continue;
        }

        // Une ligne Joueur:Alice inscrit Alice.
        if (ligne.find("Joueur:") == 0) {
            Joueur nouveauJoueur;
            nouveauJoueur.nom = nettoyer(ligne.substr(7));
            joueurs.push_back(nouveauJoueur);
        }

        // Format lisible : Action de Alice: pierre
        else if (ligne.find("Action de ") == 0) {
            size_t deuxPoints = ligne.find(':', 10);
            if (deuxPoints == string::npos) {
                cerr << "Erreur : format attendu : Action de Nom: pierre/feuille/ciseaux.\n";
                return 1;
            }

            string nom = nettoyer(ligne.substr(10, deuxPoints - 10));
            string choixTexte = enMinuscules(nettoyer(ligne.substr(deuxPoints + 1)));
            int choix = -1;

            if (choixTexte == "pierre") choix = 0;
            else if (choixTexte == "feuille") choix = 1;
            else if (choixTexte == "ciseaux") choix = 2;

            if (nom.empty() || choix == -1) {
                cerr << "Erreur : indique un joueur et pierre, feuille ou ciseaux.\n";
                return 1;
            }

            actions.push_back({nom, choix});
        }

        // Ancien format numérique conservé pour les tests déjà créés.
        else if (ligne.find("Action:") == 0) {
            int choix = stoi(ligne.substr(7));

            if (choix < 0 || choix > 2) {
                cerr << "Erreur : une action doit être 0, 1 ou 2.\n";
                return 1;
            }

            actions.push_back({"", choix});
        }
    }

    // Cette version gère au moins deux joueurs, en nombre pair.
    if (joueurs.size() < 2 || joueurs.size() % 2 != 0) {
        cerr << "Erreur : il faut un nombre pair de joueurs, au moins deux.\n";
        return 1;
    }

    // dejaJoue[a][b] indique si a et b se sont déjà rencontrés.
    vector<vector<bool>> dejaJoue(
        joueurs.size(),
        vector<bool>(joueurs.size(), false)
        );

    int prochaineAction = 0;

    // ÉTAPE 2 : faire les trois rondes.
    for (int ronde = 1; ronde <= nombreDeRondes; ronde++) {

        // Créer une liste d'indices : 0, 1, 2, etc.
        vector<int> ordre;
        for (int i = 0; i < joueurs.size(); i++) {
            ordre.push_back(i);
        }

        // Trier les indices selon les points, du meilleur score au plus bas.
        for (int i = 0; i < ordre.size(); i++) {
            for (int j = i + 1; j < ordre.size(); j++) {
                if (joueurs[ordre[j]].points > joueurs[ordre[i]].points) {
                    int temporaire = ordre[i];
                    ordre[i] = ordre[j];
                    ordre[j] = temporaire;
                }
            }
        }

        // Au début de la ronde, personne n'a encore de partenaire.
        vector<bool> dejaApparie(joueurs.size(), false);

        // ÉTAPE 3 : former les matchs.
        for (int i = 0; i < ordre.size(); i++) {
            int joueur1 = ordre[i];

            if (dejaApparie[joueur1]) {
                continue;
            }

            int joueur2 = -1;
            int meilleurEcart = 1000000;

            // Chercher un adversaire disponible jamais rencontré,
            // avec un score aussi proche que possible.
            for (int j = 0; j < ordre.size(); j++) {
                int candidat = ordre[j];

                if (candidat == joueur1 || dejaApparie[candidat]) {
                    continue;
                }

                if (dejaJoue[joueur1][candidat]) {
                    continue;
                }

                int ecart = joueurs[joueur1].points - joueurs[candidat].points;
                if (ecart < 0) {
                    ecart = -ecart;
                }

                if (ecart < meilleurEcart) {
                    meilleurEcart = ecart;
                    joueur2 = candidat;
                }
            }

            // Si aucun adversaire inédit n'est disponible,
            // choisir un adversaire disponible, même s'il y a revanche.
            if (joueur2 == -1) {
                for (int j = 0; j < ordre.size(); j++) {
                    int candidat = ordre[j];

                    if (candidat != joueur1 && !dejaApparie[candidat]) {
                        joueur2 = candidat;
                        break;
                    }
                }
            }

            dejaApparie[joueur1] = true;
            dejaApparie[joueur2] = true;

            // ÉTAPE 4 : lire les deux actions du match.
            if (prochaineAction + 1 >= actions.size()) {
                cerr << "Erreur : il manque des actions dans le fichier .in.\n";
                return 1;
            }

            Action action1 = actions[prochaineAction];
            Action action2 = actions[prochaineAction + 1];
            prochaineAction = prochaineAction + 2;

            // Vérifier que les actions nommées correspondent bien au match.
            if (!action1.nomJoueur.empty() &&
                action1.nomJoueur != joueurs[joueur1].nom) {
                cerr << "Erreur : cette action est pour " << action1.nomJoueur
                     << ", mais le match attend " << joueurs[joueur1].nom << ".\n";
                return 1;
            }
            if (!action2.nomJoueur.empty() &&
                action2.nomJoueur != joueurs[joueur2].nom) {
                cerr << "Erreur : cette action est pour " << action2.nomJoueur
                     << ", mais le match attend " << joueurs[joueur2].nom << ".\n";
                return 1;
            }

            int choix1 = action1.choix;
            int choix2 = action2.choix;

            // ÉTAPE 5 : déterminer le résultat et attribuer les points.
            int resultat = resultatDuMatch(choix1, choix2);

            if (resultat == 1) {
                joueurs[joueur1].points += 3;
                joueurs[joueur1].victoires += 1;
            } else if (resultat == -1) {
                joueurs[joueur2].points += 3;
                joueurs[joueur2].victoires += 1;
            } else {
                joueurs[joueur1].points += 1;
                joueurs[joueur2].points += 1;
            }

            // Mémoriser cette rencontre pour éviter une revanche.
            dejaJoue[joueur1][joueur2] = true;
            dejaJoue[joueur2][joueur1] = true;
        }
    }

    // ÉTAPE 6 : préparer et trier le classement final.
    vector<int> classement;
    for (int i = 0; i < joueurs.size(); i++) {
        classement.push_back(i);
    }

    for (int i = 0; i < classement.size(); i++) {
        for (int j = i + 1; j < classement.size(); j++) {
            int joueurA = classement[i];
            int joueurB = classement[j];

            bool joueurBDevant =
                joueurs[joueurB].points > joueurs[joueurA].points ||
                (joueurs[joueurB].points == joueurs[joueurA].points &&
                 joueurs[joueurB].victoires > joueurs[joueurA].victoires);

            if (joueurBDevant) {
                int temporaire = classement[i];
                classement[i] = classement[j];
                classement[j] = temporaire;
            }
        }
    }

    // ÉTAPE 7 : afficher uniquement le classement.
    for (int i = 0; i < classement.size(); i++) {
        cout << i + 1 << ":" << joueurs[classement[i]].nom << '\n';
    }
    cerr << "Scores calcules :\n";

    for (int i = 0; i < joueurs.size(); i++) {
        cerr << joueurs[i].nom
             << " : " << joueurs[i].points << " points, "
             << joueurs[i].victoires << " victoires\n";
    }
    return 0;
}