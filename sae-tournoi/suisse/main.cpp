#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#if defined(__unix__) || defined(__APPLE__)
#include <unistd.h>
#endif

using namespace std;

struct Joueur {
    string nom;
    int points = 0;
    int victoires = 0;
};

struct Action {
    string nom;
    int choix;
};

enum Jeu { PIERRE_FEUILLE_CISEAUX, DES, PILE_OU_FACE };

string nettoyer(string texte) {
    size_t debut = texte.find_first_not_of(" \t\r\n");
    if (debut == string::npos) return "";
    size_t fin = texte.find_last_not_of(" \t\r\n");
    return texte.substr(debut, fin - debut + 1);
}

string minuscules(string texte) {
    transform(texte.begin(), texte.end(), texte.begin(),
              [](unsigned char c) { return static_cast<char>(tolower(c)); });
    return texte;
}

bool lireChoix(Jeu jeu, const string& texte, int& choix) {
    string valeur = minuscules(nettoyer(texte));
    if (jeu == PIERRE_FEUILLE_CISEAUX) {
        if (valeur == "pierre") choix = 0;
        else if (valeur == "feuille") choix = 1;
        else if (valeur == "ciseaux") choix = 2;
        else return false;
        return true;
    }
    if (jeu == PILE_OU_FACE) {
        if (valeur == "pile") choix = 0;
        else if (valeur == "face") choix = 1;
        else return false;
        return true;
    }

    istringstream flux(valeur);
    string reste;
    return (flux >> choix) && !(flux >> reste) && choix >= 1 && choix <= 6;
}

string nomJeu(Jeu jeu) {
    if (jeu == PIERRE_FEUILLE_CISEAUX) return "Pierre, feuille, ciseaux";
    if (jeu == DES) return "Lanceur de dés";
    return "Pile ou face";
}

bool convertirJeu(const string& texte, Jeu& jeu) {
    string valeur = minuscules(texte);
    if (valeur == "pfc" || valeur == "pierre-feuille-ciseaux") {
        jeu = PIERRE_FEUILLE_CISEAUX;
    } else if (valeur == "des" || valeur == "dés") {
        jeu = DES;
    } else if (valeur == "pile-face" || valeur == "pile-ou-face") {
        jeu = PILE_OU_FACE;
    } else {
        return false;
    }
    return true;
}

int resultatPfc(int a, int b) {
    if (a == b) return 0;
    if ((a == 0 && b == 2) || (a == 1 && b == 0) || (a == 2 && b == 1)) return 1;
    return -1;
}

int resultatMatch(Jeu jeu, int a, int b, int resultatPiece) {
    if (jeu == PIERRE_FEUILLE_CISEAUX) return resultatPfc(a, b);
    if (jeu == DES) {
        if (a > b) return 1;
        if (b > a) return -1;
        return 0;
    }

    bool joueur1ATrouve = (a == resultatPiece);
    bool joueur2ATrouve = (b == resultatPiece);
    if (joueur1ATrouve == joueur2ATrouve) return 0;
    return joueur1ATrouve ? 1 : -1;
}

bool demanderChoix(const string& nom, Jeu jeu, int& choix) {
    string texte;
    while (true) {
        if (jeu == PIERRE_FEUILLE_CISEAUX)
            cout << "Action de " << nom << " (pierre/feuille/ciseaux) : ";
        else if (jeu == DES)
            cout << "Résultat du dé de " << nom << " (1 à 6) : ";
        else
            cout << "Prédiction de " << nom << " (pile/face) : ";

        if (!getline(cin, texte)) return false;
        if (lireChoix(jeu, texte, choix)) return true;
        cout << "Choix invalide, recommence.\n";
    }
}

bool jouerTournoi(vector<Joueur>& joueurs, Jeu jeu, bool interactif,
                  const vector<Action>& actions, const vector<int>& resultatsPiece) {
    vector<vector<bool>> dejaJoue(joueurs.size(), vector<bool>(joueurs.size(), false));
    size_t prochaineAction = 0;
    size_t prochainResultat = 0;

    for (int ronde = 1; ronde <= 3; ronde++) {
        if (interactif) cout << "\n=== Ronde " << ronde << " ===\n";

        vector<int> ordre;
        for (int i = 0; i < static_cast<int>(joueurs.size()); i++) ordre.push_back(i);
        stable_sort(ordre.begin(), ordre.end(), [&](int a, int b) {
            return joueurs[a].points > joueurs[b].points;
        });

        vector<bool> dejaApparie(joueurs.size(), false);
        for (int joueur1 : ordre) {
            if (dejaApparie[joueur1]) continue;

            int joueur2 = -1;
            int meilleurEcart = numeric_limits<int>::max();
            for (int candidat : ordre) {
                if (candidat == joueur1 || dejaApparie[candidat] || dejaJoue[joueur1][candidat]) continue;
                int ecart = abs(joueurs[joueur1].points - joueurs[candidat].points);
                if (ecart < meilleurEcart) {
                    meilleurEcart = ecart;
                    joueur2 = candidat;
                }
            }
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
                return false;
            }
            dejaApparie[joueur1] = dejaApparie[joueur2] = true;

            int choix1 = 0, choix2 = 0, resultatPiece = -1;
            if (interactif) {
                cout << joueurs[joueur1].nom << " contre " << joueurs[joueur2].nom << '\n';
                if (!demanderChoix(joueurs[joueur1].nom, jeu, choix1) ||
                    !demanderChoix(joueurs[joueur2].nom, jeu, choix2)) return false;
                if (jeu == PILE_OU_FACE) {
                    string texte;
                    while (true) {
                        cout << "Résultat fixé du lancer (pile/face) : ";
                        if (!getline(cin, texte)) return false;
                        if (lireChoix(jeu, texte, resultatPiece)) break;
                        cout << "Écris pile ou face.\n";
                    }
                }
            } else {
                if (prochaineAction + 1 >= actions.size()) {
                    cerr << "Erreur : il manque des actions dans le fichier d'entrée.\n";
                    return false;
                }
                const Action& action1 = actions[prochaineAction++];
                const Action& action2 = actions[prochaineAction++];
                if ((!action1.nom.empty() && action1.nom != joueurs[joueur1].nom) ||
                    (!action2.nom.empty() && action2.nom != joueurs[joueur2].nom)) {
                    cerr << "Erreur : les actions ne correspondent pas au match attendu entre "
                         << joueurs[joueur1].nom << " et " << joueurs[joueur2].nom << ".\n";
                    return false;
                }
                choix1 = action1.choix;
                choix2 = action2.choix;
                if (jeu == PILE_OU_FACE) {
                    if (prochainResultat >= resultatsPiece.size()) {
                        cerr << "Erreur : il manque le résultat du lancer de pièce.\n";
                        return false;
                    }
                    resultatPiece = resultatsPiece[prochainResultat++];
                }
            }

            int resultat = resultatMatch(jeu, choix1, choix2, resultatPiece);
            if (resultat == 1) {
                joueurs[joueur1].points += 3;
                joueurs[joueur1].victoires++;
            } else if (resultat == -1) {
                joueurs[joueur2].points += 3;
                joueurs[joueur2].victoires++;
            } else {
                joueurs[joueur1].points++;
                joueurs[joueur2].points++;
            }
            dejaJoue[joueur1][joueur2] = dejaJoue[joueur2][joueur1] = true;
        }
    }

    if (!interactif && (prochaineAction != actions.size() ||
                        prochainResultat != resultatsPiece.size())) {
        cerr << "Erreur : il y a trop d'actions ou de résultats dans le fichier d'entrée.\n";
        return false;
    }

    vector<int> classement;
    for (int i = 0; i < static_cast<int>(joueurs.size()); i++) classement.push_back(i);
    stable_sort(classement.begin(), classement.end(), [&](int a, int b) {
        if (joueurs[a].points != joueurs[b].points)
            return joueurs[a].points > joueurs[b].points;
        return joueurs[a].victoires > joueurs[b].victoires;
    });

    if (interactif) cout << "\n=== Classement final — " << nomJeu(jeu) << " ===\n";
    for (size_t i = 0; i < classement.size(); i++) {
        const Joueur& joueur = joueurs[classement[i]];
        cout << i + 1 << ":" << joueur.nom;
        if (interactif) cout << " — " << joueur.points << " points, " << joueur.victoires << " victoires";
        cout << '\n';
    }
    if (!interactif) {
        cerr << "Scores calcules :\n";
        for (const Joueur& joueur : joueurs)
            cerr << joueur.nom << " : " << joueur.points << " points, "
                 << joueur.victoires << " victoires\n";
    }
    return true;
}

bool lancerModeInteractif() {
    cout << "=== Tournoi suisse : choisissez un jeu ===\n"
         << "1. Pierre, feuille, ciseaux\n"
         << "2. Lanceur de dés\n"
         << "3. Pile ou face\n"
         << "Votre choix : ";
    string choixMenu;
    if (!getline(cin, choixMenu)) return false;
    Jeu jeu;
    if (choixMenu == "1") jeu = PIERRE_FEUILLE_CISEAUX;
    else if (choixMenu == "2") jeu = DES;
    else if (choixMenu == "3") jeu = PILE_OU_FACE;
    else {
        cerr << "Choix de jeu invalide.\n";
        return false;
    }

    int nombre;
    while (true) {
        cout << "Combien de joueurs ? (nombre pair, au moins 2) : ";
        string ligne;
        if (!getline(cin, ligne)) return false;
        istringstream flux(ligne);
        string reste;
        if ((flux >> nombre) && !(flux >> reste) && nombre >= 2 && nombre % 2 == 0) break;
        cout << "Entre un nombre pair supérieur ou égal à 2.\n";
    }

    vector<Joueur> joueurs;
    set<string> noms;
    for (int i = 0; i < nombre; i++) {
        string nom;
        while (true) {
            cout << "Nom du joueur " << i + 1 << " : ";
            if (!getline(cin, nom)) return false;
            nom = nettoyer(nom);
            if (!nom.empty() && noms.insert(nom).second) break;
            cout << "Le nom doit être non vide et différent des autres.\n";
        }
        joueurs.push_back({nom, 0, 0});
    }
    cout << "\nJeu choisi : " << nomJeu(jeu) << ". Aucun tirage aléatoire n'est effectué.\n";
    return jouerTournoi(joueurs, jeu, true, {}, {});
}

int main(int argc, char* argv[]) {
    bool interactif = false;
    Jeu jeu = PIERRE_FEUILLE_CISEAUX;

    if (argc == 2 && string(argv[1]) == "--jouer") {
        interactif = true;
    } else if (argc == 3 && string(argv[1]) == "--jeu") {
        if (!convertirJeu(argv[2], jeu)) {
            cerr << "Jeu inconnu. Utilise pfc, des ou pile-face.\n";
            return 1;
        }
    } else if (argc == 1) {
#if defined(__unix__) || defined(__APPLE__)
        interactif = isatty(STDIN_FILENO);
#endif
    } else {
        cerr << "Usage : suisse [--jouer | --jeu pfc|des|pile-face]\n";
        return 1;
    }

    if (interactif) return lancerModeInteractif() ? 0 : 1;

    vector<Joueur> joueurs;
    vector<Action> actions;
    vector<int> resultatsPiece;
    string ligne;
    while (getline(cin, ligne)) {
        ligne = nettoyer(ligne);
        if (ligne.empty() || ligne[0] == '#') continue;
        if (ligne.rfind("Joueur:", 0) == 0) {
            string nom = nettoyer(ligne.substr(7));
            if (nom.empty()) {
                cerr << "Erreur : le nom d'un joueur est vide.\n";
                return 1;
            }
            joueurs.push_back({nom, 0, 0});
        } else if (ligne.rfind("Action de ", 0) == 0) {
            size_t deuxPoints = ligne.find(':', 10);
            int choix;
            if (deuxPoints == string::npos ||
                !lireChoix(jeu, ligne.substr(deuxPoints + 1), choix)) {
                cerr << "Erreur : action invalide pour " << nomJeu(jeu) << ".\n";
                return 1;
            }
            actions.push_back({nettoyer(ligne.substr(10, deuxPoints - 10)), choix});
        } else if (ligne.rfind("Action:", 0) == 0 && jeu == PIERRE_FEUILLE_CISEAUX) {
            int choix = -1;
            string valeur = nettoyer(ligne.substr(7));
            istringstream flux(valeur);
            string reste;
            if (!(flux >> choix) || (flux >> reste) || choix < 0 || choix > 2) {
                cerr << "Erreur : action numérique PFC attendue : 0, 1 ou 2.\n";
                return 1;
            }
            actions.push_back({"", choix});
        } else if (ligne.rfind("Resultat:", 0) == 0 && jeu == PILE_OU_FACE) {
            int resultat;
            if (!lireChoix(jeu, ligne.substr(9), resultat)) {
                cerr << "Erreur : le résultat doit être Pile ou Face.\n";
                return 1;
            }
            resultatsPiece.push_back(resultat);
        } else {
            cerr << "Erreur : ligne inconnue : " << ligne << '\n';
            return 1;
        }
    }

    if (joueurs.size() < 2 || joueurs.size() % 2 != 0) {
        cerr << "Erreur : il faut un nombre pair de joueurs, au moins deux.\n";
        return 1;
    }
    set<string> noms;
    for (const Joueur& joueur : joueurs) {
        if (!noms.insert(joueur.nom).second) {
            cerr << "Erreur : deux joueurs ont le même nom : " << joueur.nom << ".\n";
            return 1;
        }
    }
    return jouerTournoi(joueurs, jeu, false, actions, resultatsPiece) ? 0 : 1;
}
