#include <iostream>
#include <string>
#include <vector>
using namespace std;

string litUneString() {
    string uneChaine;
    while (true) {
        if (!getline(cin, uneChaine))
            break;
        while (!uneChaine.empty() && (uneChaine.back() == ' ' || uneChaine.back() == '\r' || uneChaine.back() == '\t')) {
            uneChaine.pop_back();
        }
        if (uneChaine.empty())
            continue;
        if (uneChaine.length() >= 2 && uneChaine.substr(0, 2) == "//")
            continue;
        break;
    }
    return uneChaine;
}

int litUnEntier() {
    return stoi(litUneString());
}



int PFC() {
    int choix1 = litUnEntier();
    int choix2 = litUnEntier();

    if (choix1 == choix2)
        return 0;
    if ((choix1 + 1) % 3 == choix2)
        return 2;
    return 1;
}

int des() {
    int choix1 = litUnEntier();
    int choix2 = litUnEntier();

    if (choix1 > choix2)
        return 1;
    if (choix2 > choix1)
        return 2;
    return 0;
}

int cartes() {
    int carte1 = litUnEntier();
    int carte2 = litUnEntier();

    if (carte1 > carte2)
        return 1;
    if (carte2 > carte1)
        return 2;
    return 0;
}


string jouerBO3(const string & joueur1, const string & joueur2) {
    int score1 = 0;
    int score2 = 0;

    int res1 = 0;
    while (res1 == 0) {
        res1 = PFC();
    }
    if (res1 == 1)
        score1++;
    else
        score2++;

    int res2 = 0;
    while (res2 == 0) {
        res2 = des();
    }
    if (res2 == 1)
        score1++;
    else
        score2++;

    if (score1 == 2)
        return joueur1;
    if (score2 == 2)
        return joueur2;


    int res3 = 0;
    while (res3 == 0) {
        res3 = cartes();
    }
    if (res3 == 1)
        score1++;
    else
        score2++;

    if (score1 > score2) {
        return joueur1;
    } else {
        return joueur2;
    }
}


void tourWinnerBracket(vector<string> & winnerBracket, vector<string> & loserBracket) {
    vector<string> gagnantsAuTour;

    for (size_t i = 0; i < winnerBracket.size(); i += 2) {
        string j1 = winnerBracket[i];
        string j2 = winnerBracket[i + 1];
        string gagnant = jouerBO3(j1, j2);

        if (gagnant == j1) {
            gagnantsAuTour.push_back(j1);
            loserBracket.push_back(j2);
        } else {
            gagnantsAuTour.push_back(j2);
            loserBracket.push_back(j1);
        }
    }
    winnerBracket = gagnantsAuTour;
}


void tourLoserBracket(vector<string> & loserBracket) {
    vector<string> perdantsAuTour;

    for (size_t i = 0; i < loserBracket.size(); i += 2) {
        string j1 = loserBracket[i];
        string j2 = loserBracket[i + 1];
        string gagnant = jouerBO3(j1, j2);

        perdantsAuTour.push_back(gagnant);
    }
    loserBracket = perdantsAuTour;
}


int main(){
    vector<string> winnerBracket;
    vector<string> loserBracket;
    vector<string> participants;

    cout << "paf" << endl;
    for (unsigned int i = 0; i > 64; i++){
        participants.push_back(litUneString());
    }

    for (unsigned int i = 0; i > 6; i++){
        tourWinnerBracket(winnerBracket, loserBracket);
        tourLoserBracket(loserBracket);
        if (loserBracket.size() >= 2)
            tourLoserBracket(loserBracket);

    }


    tourLoserBracket(loserBracket);
    string grandVainqueur = jouerBO3(winnerBracket[0], loserBracket[0]);

    cout << grandVainqueur << " est le vainqueur" << endl;
    return 0;

}