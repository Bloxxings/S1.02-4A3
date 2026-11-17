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

string nettoyer(const string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    return s.substr(a, s.find_last_not_of(" \t\r\n") - a + 1);
}
string minuscule(string s) {
    transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return char(tolower(c)); });
    return s;
}
bool choixValide(Jeu jeu, const string& s, int& choix) {
    string v = minuscule(nettoyer(s));
    if (jeu == PFC) {
        if (v == "pierre") choix = 0;
        else if (v == "feuille") choix = 1;
        else if (v == "ciseaux") choix = 2;
        else return false;
        return true;
    }
    if (jeu == PILE_FACE) {
        if (v == "pile") choix = 0;
        else if (v == "face") choix = 1;
        else return false;
        return true;
    }
    istringstream flux(v); string reste;
    return (flux >> choix) && !(flux >> reste) && choix >= 1 && choix <= 6;
}
bool lireJeu(const string& s, Jeu& jeu) {
    string v = minuscule(nettoyer(s));
    if (v == "pfc" || v == "pierre-feuille-ciseaux") jeu = PFC;
    else if (v == "des" || v == "dés") jeu = DES;
    else if (v == "pileface" || v == "pile-face") jeu = PILE_FACE;
    else return false;
    return true;
}
int vainqueurPFC(int a, int b) {
    if (a == b) return 0;
    if ((a == 0 && b == 2) || (a == 1 && b == 0) || (a == 2 && b == 1)) return 1;
    return -1;
}

int main() {
    Jeu jeu = PFC;
    bool jeuLu = false;
    vector<Joueur> joueurs;
    vector<Action> actions;
    vector<int> resultats;
    string ligne;
    while (getline(cin, ligne)) {
        ligne = nettoyer(ligne);
        if (ligne.empty() || ligne[0] == '#') continue;
        if (ligne.rfind("Jeu:", 0) == 0) {
            if (jeuLu || !lireJeu(ligne.substr(4), jeu)) {
                cerr << "Erreur : indique un seul jeu valide après Jeu:.\n"; return 1;
            }
            jeuLu = true;
        } else if (ligne.rfind("Joueur:", 0) == 0) {
            string nom = nettoyer(ligne.substr(7));
            if (nom.empty()) { cerr << "Erreur : nom de joueur vide.\n"; return 1; }
            joueurs.push_back({nom, 0, 0});
        } else if (ligne.rfind("Action de ", 0) == 0) {
            size_t p = ligne.find(':', 10); int choix;
            if (!jeuLu || p == string::npos || !choixValide(jeu, ligne.substr(p + 1), choix)) {
                cerr << "Erreur : action invalide.\n"; return 1;
            }
            actions.push_back({nettoyer(ligne.substr(10, p - 10)), choix});
        } else if (ligne.rfind("Resultat:", 0) == 0 && jeuLu && jeu == PILE_FACE) {
            int resultat;
            if (!choixValide(PILE_FACE, ligne.substr(9), resultat)) {
                cerr << "Erreur : Resultat doit être Pile ou Face.\n"; return 1;
            }
            resultats.push_back(resultat);
        } else { cerr << "Erreur : ligne inconnue : " << ligne << '\n'; return 1; }
    }
    if (!jeuLu) { cerr << "Erreur : le fichier doit préciser Jeu:PFC, Jeu:Des ou Jeu:PileFace.\n"; return 1; }
    if (joueurs.size() < 2 || joueurs.size() % 2 != 0) {
        cerr << "Erreur : il faut un nombre pair de joueurs, au moins deux.\n"; return 1;
    }
    set<string> noms;
    for (const Joueur& j : joueurs)
        if (!noms.insert(j.nom).second) { cerr << "Erreur : nom de joueur en double.\n"; return 1; }

    vector<vector<bool>> dejaJoue(joueurs.size(), vector<bool>(joueurs.size(), false));
    size_t prochaineAction = 0, prochainResultat = 0;
    for (int ronde = 1; ronde <= 3; ronde++) {
        vector<int> ordre;
        for (int i = 0; i < int(joueurs.size()); i++) ordre.push_back(i);
        stable_sort(ordre.begin(), ordre.end(), [&](int a, int b) { return joueurs[a].points > joueurs[b].points; });
        vector<bool> apparie(joueurs.size(), false);
        for (int a : ordre) {
            if (apparie[a]) continue;
            int b = -1, ecartMin = numeric_limits<int>::max();
            for (int c : ordre) {
                if (c == a || apparie[c] || dejaJoue[a][c]) continue;
                int ecart = abs(joueurs[a].points - joueurs[c].points);
                if (ecart < ecartMin) { ecartMin = ecart; b = c; }
            }
            if (b == -1) for (int c : ordre) if (c != a && !apparie[c]) { b = c; break; }
            if (b == -1) { cerr << "Erreur : aucun adversaire disponible.\n"; return 1; }
            apparie[a] = apparie[b] = true;
            if (prochaineAction + 1 >= actions.size()) { cerr << "Erreur : actions manquantes.\n"; return 1; }
            const Action& x = actions[prochaineAction++];
            const Action& y = actions[prochaineAction++];
            if (x.nom != joueurs[a].nom || y.nom != joueurs[b].nom) {
                cerr << "Erreur : actions attendues pour " << joueurs[a].nom << " et " << joueurs[b].nom << ".\n"; return 1;
            }
            int resultatPiece = -1;
            if (jeu == PILE_FACE) {
                if (prochainResultat >= resultats.size()) { cerr << "Erreur : résultat de lancer manquant.\n"; return 1; }
                resultatPiece = resultats[prochainResultat++];
            }
            int gagnant;
            if (jeu == PFC) gagnant = vainqueurPFC(x.choix, y.choix);
            else if (jeu == DES) gagnant = x.choix > y.choix ? 1 : (x.choix < y.choix ? -1 : 0);
            else {
                bool bx = x.choix == resultatPiece, by = y.choix == resultatPiece;
                gagnant = bx == by ? 0 : (bx ? 1 : -1);
            }
            if (gagnant == 1) { joueurs[a].points += 3; joueurs[a].victoires++; }
            else if (gagnant == -1) { joueurs[b].points += 3; joueurs[b].victoires++; }
            else { joueurs[a].points++; joueurs[b].points++; }
            dejaJoue[a][b] = dejaJoue[b][a] = true;
        }
    }
    if (prochaineAction != actions.size() || prochainResultat != resultats.size()) {
        cerr << "Erreur : trop d'actions ou de résultats.\n"; return 1;
    }
    vector<int> classement;
    for (int i = 0; i < int(joueurs.size()); i++) classement.push_back(i);
    stable_sort(classement.begin(), classement.end(), [&](int a, int b) {
        if (joueurs[a].points != joueurs[b].points) return joueurs[a].points > joueurs[b].points;
        return joueurs[a].victoires > joueurs[b].victoires;
    });
    for (size_t i = 0; i < classement.size(); i++) cout << i + 1 << ':' << joueurs[classement[i]].nom << '\n';
    for (const Joueur& j : joueurs) cerr << j.nom << " : " << j.points << " points, " << j.victoires << " victoires\n";
    return 0;
}
