#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

using namespace std;

struct Joueur { string nom; int points = 0; int victoires = 0; };
struct Action { string nom; int choix; };
enum Jeu { PFC, DES, PILE_FACE };

// cebqu'in lit dans le fichier pour un jeu :joueurs,actions et les tirages de pièce
struct Tournoi {
    Jeu jeu = PFC;
    bool jeuLu = false;
    vector<Joueur> joueurs;
    vector<Action> actions;
    vector<int> resultats;
};

// Enlève les espaces et les tabulations au début et à la fin d'un texte
string enleverEspaces(const string& s) {
    size_t debut = s.find_first_not_of(" \t\r\n");
    if (debut == string::npos) return "";
    return s.substr(debut, s.find_last_not_of(" \t\r\n") - debut + 1);
}

// Met un texte en minuscules
string enMinuscules(string s) {
    transform(s.begin(), s.end(), s.begin(),
              [](unsigned char c) { return static_cast<char>(tolower(c)); });
    return s;
}

// pour lire le type de jeu écrit après jeu
void lireTypeDeJeu(const string& texte, Jeu& jeu) {
    string valeur = enMinuscules(enleverEspaces(texte));
    if (valeur == "des" || valeur == "dés") jeu = DES;
    else if (valeur == "pileface" || valeur == "pile-face") jeu = PILE_FACE;
    else jeu = PFC;
}

// choix -->  pierre 0, feuille 1, ciseaux 2 / pile 0, face 1 / dé 1 à 6
void lireChoix(Jeu jeu, const string& texte, int& choix) {
    string valeur = enMinuscules(enleverEspaces(texte));
    if (jeu == PFC) choix = valeur == "pierre" ? 0 : (valeur == "feuille" ? 1 : 2);
    else if (jeu == PILE_FACE) choix = valeur == "pile" ? 0 : 1;
    else choix = stoi(valeur);
}

// Renvoie 1 si a gagne, -1 si b gagne, 0 si égalité
int gagnantPFC(int a, int b) {
    if (a == b) return 0;
    if ((a == 0 && b == 2) || (a == 1 && b == 0) || (a == 2 && b == 1)) return 1;
    return -1;
}

// Lit une ligne du fichier et la range
void lireUneLigne(Tournoi& t, const string& ligne) {
    if (ligne.rfind("Joueur:", 0) == 0) {
        t.joueurs.push_back({enleverEspaces(ligne.substr(7)), 0, 0});
    } else if (ligne.rfind("Action de ", 0) == 0) {
        size_t deuxPoints = ligne.find(':', 10);
        int choix;
        lireChoix(t.jeu, ligne.substr(deuxPoints + 1), choix);
        t.actions.push_back({enleverEspaces(ligne.substr(10, deuxPoints - 10)), choix});
    } else if (ligne.rfind("Resultat:", 0) == 0 && t.jeu == PILE_FACE) {
        int resultat;
        lireChoix(PILE_FACE, ligne.substr(9), resultat);
        t.resultats.push_back(resultat);
    }
}

// cette fonction est pour jouer les 3 rondes du système suisse, puis ajouter le classement final du jeu à "classements"
void jouerTournoi(Tournoi& t, vector<vector<Joueur>>& classements) {
    vector<Joueur>& joueurs = t.joueurs;
    // dejaJoue[a][b] vaut true si les joueurs a et b se sont déjà affrontés
    vector<vector<bool>> dejaJoue(joueurs.size(), vector<bool>(joueurs.size(), false));
    // Position de la prochaine action et du prochain tirage à lire dans les listes
    size_t prochaineAction = 0, prochainResultat = 0;
    // 3 rondes
    for (int ronde = 1; ronde <= 3; ronde++) {
        // On range les num des joueurs du plus au moins de pts
        vector<int> ordre;
        for (int i = 0; i < static_cast<int>(joueurs.size()); i++) ordre.push_back(i);
        sort(ordre.begin(), ordre.end(), [&](int a, int b) {
            if (joueurs[a].points != joueurs[b].points) return joueurs[a].points > joueurs[b].points;
            return a < b;   // si égalité, on garde l'ordre de départ
        });
        // dejaApparie : les joueurs déjà placés dans une paire pendant cette ronde
        vector<bool> dejaApparie(joueurs.size(), false);
        // Pour chaque joueur pas encore apparié, on lui cherche un adversaire
        for (int a : ordre) {
            if (dejaApparie[a]) continue;
            // Adversaire = c'est celui dont les pts sont les plus proches parmi ceux qu'on n'a pas déjà affrontés
            int b = -1, meilleurEcart = numeric_limits<int>::max();
            for (int candidat : ordre) {
                if (candidat == a || dejaApparie[candidat] || dejaJoue[a][candidat]) continue;
                int ecart = abs(joueurs[a].points - joueurs[candidat].points);
                if (ecart < meilleurEcart) { meilleurEcart = ecart; b = candidat; }
            }
            // Si on a déjà affronté tout le monde : revanche contre le premier joueur libre
            if (b == -1) {
                for (int candidat : ordre)
                    if (candidat != a && !dejaApparie[candidat]) { b = candidat; break; }
            }
            // Mtn deux joueurs sont dans une paire
            dejaApparie[a] = dejaApparie[b] = true;
            // On lit les actions des deux joueurs, dans l'ordre du fichier ttxt
            const Action& x = t.actions[prochaineAction++];
            const Action& y = t.actions[prochaineAction++];
            // Pile ou face : on lit aussi le tirage de la pièce
            int resultatPiece = -1;
            if (t.jeu == PILE_FACE) resultatPiece = t.resultats[prochainResultat++];
            // gagnant : 1 si a gagne, -1 si b gagne, 0 si égalité
            int gagnant;
            if (t.jeu == PFC) gagnant = gagnantPFC(x.choix, y.choix);
            else if (t.jeu == DES) gagnant = x.choix > y.choix ? 1 : (x.choix < y.choix ? -1 : 0);
            else {
                // Pile ou face : celui qui devine juste gagne. si les deux ont le mm choix : égalité
                bool xJuste = x.choix == resultatPiece;
                bool yJuste = y.choix == resultatPiece;
                gagnant = xJuste == yJuste ? 0 : (xJuste ? 1 : -1);
            }
            // Victoire = 3 points et une victoire de plus. Égalité = 1 point chacun
            if (gagnant == 1) { joueurs[a].points += 3; joueurs[a].victoires++; }
            else if (gagnant == -1) { joueurs[b].points += 3; joueurs[b].victoires++; }
            else { joueurs[a].points++; joueurs[b].points++; }
            // noter et enregistrer  que a et b se sont affrontés
            dejaJoue[a][b] = dejaJoue[b][a] = true;
        }
    }
    // Classement final du jeu; points, victoires, et par ordre de départ
    vector<int> classement;
    for (int i = 0; i < static_cast<int>(joueurs.size()); i++) classement.push_back(i);
    sort(classement.begin(), classement.end(), [&](int a, int b) {
        if (joueurs[a].points != joueurs[b].points) return joueurs[a].points > joueurs[b].points;
        if (joueurs[a].victoires != joueurs[b].victoires) return joueurs[a].victoires > joueurs[b].victoires;
        return a < b;
    });
    // On range les joueurs dans cet ordre et on garde le classement du jeu
    vector<Joueur> joueursClasses;
    for (int indice : classement) joueursClasses.push_back(joueurs[indice]);
    classements.push_back(joueursClasses);
}

// Le nom du jeu à afficher
string nomDuJeu(Jeu jeu) {
    if (jeu == PFC) return "Pierre-feuille-ciseaux";
    if (jeu == DES) return "Des";
    return "Pile ou face";
}

// Un joueur avec ses points dans chacun des 3 jeux pour le classement général ensuite
struct JoueurTotal {
    string nom;
    int pointsPFC = 0;
    int pointsDes = 0;
    int pointsPileFace = 0;
    int victoires = 0;
    int place = 0;   // rang dans le premier jeu, pour départager
    int total() const { return pointsPFC + pointsDes + pointsPileFace; }
};

// Le programme : 1) on lit le fichier  2) on joue chaque jeu et on affiche son classement  3) on affiche le classement général
int main() {
    Tournoi tournoi;
    vector<vector<Joueur>> classements;
    vector<Jeu> jeux;
    string ligne;
    // 1) Lecture du fichier ligne par ligne
    while (getline(cin, ligne)) {
        ligne = enleverEspaces(ligne);
        // ignorer les lignes vides et les commentaires qui commencent par #
        if (ligne.empty() || ligne[0] == '#') continue;
        // Nouvelle ligne "Jeu:" : on joue le jeu précédent (s'il y en a un), puis on en commence un nouveau
        if (ligne.rfind("Jeu:", 0) == 0) {
            if (tournoi.jeuLu) {
                jouerTournoi(tournoi, classements);
                jeux.push_back(tournoi.jeu);
                tournoi = Tournoi();
            }
            lireTypeDeJeu(ligne.substr(4), tournoi.jeu);
            tournoi.jeuLu = true;
        } else {
            lireUneLigne(tournoi, ligne);
        }
    }
    // On joue le dernier jeu du fichier
    jouerTournoi(tournoi, classements);
    jeux.push_back(tournoi.jeu);
    // Classement général : on part des joueurs dans l'ordre du premier jeu
    vector<JoueurTotal> global;
    for (size_t i = 0; i < classements[0].size(); i++) {
        JoueurTotal g;
        g.nom = classements[0][i].nom;
        g.place = i;
        global.push_back(g);
    }
    // 2) j'affiche le classement de chaque jeu et je note les points de chaque joueur a coté
    cout << "CLASSEMENTS PAR JEU\n";
    for (size_t bloc = 0; bloc < jeux.size(); bloc++) {
        cout << "\n" << nomDuJeu(jeux[bloc]) << " :\n";
        for (size_t rang = 0; rang < classements[bloc].size(); rang++) {
            const Joueur& joueur = classements[bloc][rang];
            cout << rang + 1 << ":" << joueur.nom << " - " << joueur.points << " points\n";
            // On retrouve ce joueur dans le classement général (je recherche par son nom)
            auto trouve = find_if(global.begin(), global.end(), [&](const JoueurTotal& g) {
                return g.nom == joueur.nom;
            });
            if (jeux[bloc] == PFC) trouve->pointsPFC = joueur.points;
            else if (jeux[bloc] == DES) trouve->pointsDes = joueur.points;
            else trouve->pointsPileFace = joueur.points;
            trouve->victoires += joueur.victoires;
        }
    }
    // ranger le classement général : total des points, victoires et place dans le premier jeu
    sort(global.begin(), global.end(), [](const JoueurTotal& a, const JoueurTotal& b) {
        if (a.total() != b.total()) return a.total() > b.total();
        if (a.victoires != b.victoires) return a.victoires > b.victoires;
        return a.place < b.place;
    });
    // 3) Affichage du classement général
    cout << "\nCLASSEMENT GENERAL (points PFC + Des + Pile ou face) :\n";
    for (size_t rang = 0; rang < global.size(); rang++) {
        const JoueurTotal& joueur = global[rang];
        cout << rang + 1 << ":" << joueur.nom << " - " << joueur.total()
             << " points\n";
    }
    return 0;
}