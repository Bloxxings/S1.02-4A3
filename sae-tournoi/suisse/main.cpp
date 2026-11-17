#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct Joueur { string nom; int points = 0; int victoires = 0; };
struct Action { string nom; int choix; };
enum Jeu { PFC, DES, PILE_FACE };

struct Tournoi {
    Jeu jeu = PFC;
    bool jeuLu = false;
    vector<Joueur> joueurs;
    vector<Action> actions;
    vector<int> resultats;
};

string nettoyer(const string& s) {
    size_t debut = s.find_first_not_of(" \t\r\n");
    if (debut == string::npos) return "";
    return s.substr(debut, s.find_last_not_of(" \t\r\n") - debut + 1);
}

string minuscules(string s) {
    transform(s.begin(), s.end(), s.begin(),
              [](unsigned char c) { return static_cast<char>(tolower(c)); });
    return s;
}

bool lireJeu(const string& texte, Jeu& jeu) {
    string valeur = minuscules(nettoyer(texte));
    if (valeur == "pfc" || valeur == "pierre-feuille-ciseaux") jeu = PFC;
    else if (valeur == "des" || valeur == "dés") jeu = DES;
    else if (valeur == "pileface" || valeur == "pile-face") jeu = PILE_FACE;
    else return false;
    return true;
}

bool lireChoix(Jeu jeu, const string& texte, int& choix) {
    string valeur = minuscules(nettoyer(texte));
    if (jeu == PFC) {
        if (valeur == "pierre") choix = 0;
        else if (valeur == "feuille") choix = 1;
        else if (valeur == "ciseaux") choix = 2;
        else return false;
        return true;
    }
    if (jeu == PILE_FACE) {
        if (valeur == "pile") choix = 0;
        else if (valeur == "face") choix = 1;
        else return false;
        return true;
    }
    istringstream flux(valeur);
    string reste;
    return (flux >> choix) && !(flux >> reste) && choix >= 1 && choix <= 6;
}

int resultatPFC(int a, int b) {
    if (a == b) return 0;
    if ((a == 0 && b == 2) || (a == 1 && b == 0) || (a == 2 && b == 1)) return 1;
    return -1;
}

bool traiterLigne(Tournoi& t, const string& ligne) {
    if (ligne.rfind("Joueur:", 0) == 0) {
        string nom = nettoyer(ligne.substr(7));
        if (nom.empty()) { cerr << "Erreur : nom de joueur vide.\n"; return false; }
        t.joueurs.push_back({nom, 0, 0});
    } else if (ligne.rfind("Action de ", 0) == 0) {
        size_t deuxPoints = ligne.find(':', 10);
        int choix;
        if (deuxPoints == string::npos || !lireChoix(t.jeu, ligne.substr(deuxPoints + 1), choix)) {
            cerr << "Erreur : action invalide pour le jeu choisi.\n";
            return false;
        }
        t.actions.push_back({nettoyer(ligne.substr(10, deuxPoints - 10)), choix});
    } else if (ligne.rfind("Resultat:", 0) == 0 && t.jeu == PILE_FACE) {
        int resultat;
        if (!lireChoix(PILE_FACE, ligne.substr(9), resultat)) {
            cerr << "Erreur : Resultat doit être Pile ou Face.\n";
            return false;
        }
        t.resultats.push_back(resultat);
    } else {
        cerr << "Erreur : ligne inconnue : " << ligne << '\n';
        return false;
    }
    return true;
}

bool calculerTournoi(Tournoi& t, vector<vector<Joueur>>& classements) {
    vector<Joueur>& joueurs = t.joueurs;
    if (joueurs.size() < 2 || joueurs.size() % 2 != 0) {
        cerr << "Erreur : il faut un nombre pair de joueurs, au moins deux.\n";
        return false;
    }
    set<string> noms;
    for (const Joueur& joueur : joueurs) {
        if (!noms.insert(joueur.nom).second) {
            cerr << "Erreur : deux joueurs ont le même nom dans un tournoi.\n";
            return false;
        }
    }

    vector<vector<bool>> dejaJoue(joueurs.size(), vector<bool>(joueurs.size(), false));
    size_t prochaineAction = 0, prochainResultat = 0;
    for (int ronde = 1; ronde <= 3; ronde++) {
        vector<int> ordre;
        for (int i = 0; i < static_cast<int>(joueurs.size()); i++) ordre.push_back(i);
        stable_sort(ordre.begin(), ordre.end(), [&](int a, int b) {
            return joueurs[a].points > joueurs[b].points;
        });
        vector<bool> dejaApparie(joueurs.size(), false);

        for (int a : ordre) {
            if (dejaApparie[a]) continue;
            int b = -1, meilleurEcart = numeric_limits<int>::max();
            for (int candidat : ordre) {
                if (candidat == a || dejaApparie[candidat] || dejaJoue[a][candidat]) continue;
                int ecart = abs(joueurs[a].points - joueurs[candidat].points);
                if (ecart < meilleurEcart) { meilleurEcart = ecart; b = candidat; }
            }
            if (b == -1) {
                for (int candidat : ordre)
                    if (candidat != a && !dejaApparie[candidat]) { b = candidat; break; }
            }
            if (b == -1) { cerr << "Erreur : aucun adversaire disponible.\n"; return false; }
            dejaApparie[a] = dejaApparie[b] = true;
            if (prochaineAction + 1 >= t.actions.size()) {
                cerr << "Erreur : actions manquantes.\n";
                return false;
            }
            const Action& x = t.actions[prochaineAction++];
            const Action& y = t.actions[prochaineAction++];
            if (x.nom != joueurs[a].nom || y.nom != joueurs[b].nom) {
                cerr << "Erreur : actions attendues pour " << joueurs[a].nom
                     << " et " << joueurs[b].nom << ".\n";
                return false;
            }

            int resultatPiece = -1;
            if (t.jeu == PILE_FACE) {
                if (prochainResultat >= t.resultats.size()) {
                    cerr << "Erreur : résultat de lancer manquant.\n";
                    return false;
                }
                resultatPiece = t.resultats[prochainResultat++];
            }
            int gagnant;
            if (t.jeu == PFC) gagnant = resultatPFC(x.choix, y.choix);
            else if (t.jeu == DES) gagnant = x.choix > y.choix ? 1 : (x.choix < y.choix ? -1 : 0);
            else {
                bool xJuste = x.choix == resultatPiece;
                bool yJuste = y.choix == resultatPiece;
                gagnant = xJuste == yJuste ? 0 : (xJuste ? 1 : -1);
            }
            if (gagnant == 1) { joueurs[a].points += 3; joueurs[a].victoires++; }
            else if (gagnant == -1) { joueurs[b].points += 3; joueurs[b].victoires++; }
            else { joueurs[a].points++; joueurs[b].points++; }
            dejaJoue[a][b] = dejaJoue[b][a] = true;
        }
    }

    if (prochaineAction != t.actions.size() || prochainResultat != t.resultats.size()) {
        cerr << "Erreur : trop d'actions ou de résultats.\n";
        return false;
    }
    vector<int> classement;
    for (int i = 0; i < static_cast<int>(joueurs.size()); i++) classement.push_back(i);
    stable_sort(classement.begin(), classement.end(), [&](int a, int b) {
        if (joueurs[a].points != joueurs[b].points) return joueurs[a].points > joueurs[b].points;
        return joueurs[a].victoires > joueurs[b].victoires;
    });
    vector<Joueur> joueursClasses;
    for (int indice : classement) joueursClasses.push_back(joueurs[indice]);
    classements.push_back(joueursClasses);
    return true;
}

string nomJeu(Jeu jeu) {
    if (jeu == PFC) return "Pierre-feuille-ciseaux";
    if (jeu == DES) return "Des";
    return "Pile ou face";
}

struct JoueurGlobal {
    string nom;
    int pointsPFC = 0;
    int pointsDes = 0;
    int pointsPileFace = 0;
    int victoires = 0;
    int total() const { return pointsPFC + pointsDes + pointsPileFace; }
};

int main() {
    Tournoi tournoi;
    vector<vector<Joueur>> classements;
    vector<Jeu> jeux;
    bool auMoinsUnTournoi = false;
    string ligne;
    while (getline(cin, ligne)) {
        ligne = nettoyer(ligne);
        if (ligne.empty() || ligne[0] == '#') continue;
        if (ligne.rfind("Jeu:", 0) == 0) {
            if (tournoi.jeuLu) {
                if (!calculerTournoi(tournoi, classements)) return 1;
                jeux.push_back(tournoi.jeu);
                tournoi = Tournoi();
            }
            if (!lireJeu(ligne.substr(4), tournoi.jeu)) {
                cerr << "Erreur : jeu invalide après Jeu:.\n";
                return 1;
            }
            tournoi.jeuLu = true;
            auMoinsUnTournoi = true;
        } else {
            if (!tournoi.jeuLu) {
                cerr << "Erreur : chaque bloc doit commencer par Jeu:.\n";
                return 1;
            }
            if (!traiterLigne(tournoi, ligne)) return 1;
        }
    }
    if (!auMoinsUnTournoi) {
        cerr << "Erreur : le fichier doit contenir au moins un bloc Jeu:.\n";
        return 1;
    }
    if (!calculerTournoi(tournoi, classements)) return 1;
    jeux.push_back(tournoi.jeu);

    if (jeux.size() != 3) {
        cerr << "Erreur : il faut un bloc pour chacun des trois jeux.\n";
        return 1;
    }
    vector<bool> jeuPresent(3, false);
    for (Jeu jeu : jeux) {
        if (jeuPresent[jeu]) {
            cerr << "Erreur : chaque jeu doit apparaître une seule fois.\n";
            return 1;
        }
        jeuPresent[jeu] = true;
    }

    vector<string> noms;
    for (const Joueur& joueur : classements[0]) noms.push_back(joueur.nom);
    vector<JoueurGlobal> global;
    for (const string& nom : noms) global.push_back({nom});

    cout << "CLASSEMENTS PAR JEU\n";
    for (size_t bloc = 0; bloc < jeux.size(); bloc++) {
        cout << "\n" << nomJeu(jeux[bloc]) << " :\n";
        for (size_t rang = 0; rang < classements[bloc].size(); rang++) {
            const Joueur& joueur = classements[bloc][rang];
            cout << rang + 1 << ":" << joueur.nom << " - " << joueur.points << " points\n";
            auto trouve = find_if(global.begin(), global.end(), [&](const JoueurGlobal& g) {
                return g.nom == joueur.nom;
            });
            if (trouve == global.end()) {
                cerr << "Erreur : les joueurs doivent être identiques pour les trois jeux.\n";
                return 1;
            }
            if (jeux[bloc] == PFC) trouve->pointsPFC = joueur.points;
            else if (jeux[bloc] == DES) trouve->pointsDes = joueur.points;
            else trouve->pointsPileFace = joueur.points;
            trouve->victoires += joueur.victoires;
        }
    }

    for (const auto& partie : classements) {
        if (partie.size() != global.size()) {
            cerr << "Erreur : les joueurs doivent être identiques pour les trois jeux.\n";
            return 1;
        }
    }
    stable_sort(global.begin(), global.end(), [](const JoueurGlobal& a, const JoueurGlobal& b) {
        if (a.total() != b.total()) return a.total() > b.total();
        return a.victoires > b.victoires;
    });

    cout << "\nCLASSEMENT GENERAL (points PFC + Des + Pile ou face) :\n";
    for (size_t rang = 0; rang < global.size(); rang++) {
        const JoueurGlobal& joueur = global[rang];
        cout << rang + 1 << ":" << joueur.nom << " - " << joueur.total()
             << " points\n";
    }
    return 0;
}
