#include <iostream> //bibliotheque pour le flux d'entrée/sortie
#include <vector> //pour les tableaux
#include <algorithm> //permet, dans ce code, de trier les joueurs dans l'ordre qu'on veut avec comme parametre compare2part

using namespace std; //pour éviter de mettre std:: a plusieurs lignes

string litUneString () //fonction pour lire les lignes du doc entrée
{
    string uneChaine; //déclare uneChaine en string
    while (true)  //tant qu'on a pas finis de parcourir la fin du fichier/arrivés a un element inattendu
    {
        getline(cin, uneChaine);
        if ((!cin) || (uneChaine.substr(0,2) != "&&")) break; //si le commentaire commence par & ou si l'entrée est inattendue, on sors de la boucle
    }
    return uneChaine; //la fonction renvoie une chaine
}

int litUnEntier () //fonction pour lire un entier en évitant les commentaires commencant par &
{
    string uneChaine; //déclare uneChaine en string
    while (true) //tant qu'on a pas finis de parcourir la fin du fichier/on arrive a un element inattendu
    {
        getline(cin, uneChaine);
        if ((!cin) || (uneChaine.substr(0,2) != "&&")) break; //si le commentaire ne commence pas par & ou si l'entrée est inattendue, on sors de la boucle
    }
    return stoi(uneChaine); //convertit la chaine en entier et la renvoie
}

struct participant //on déclare un participant avec son prénom et son nombre de points
{
    string prenom;
    int points;
};

bool compare2part (const participant & p1, const participant & p2)
{
    return p1.points > p2.points; //renvoie un booleen pour savoir qui a le plus de points
}

void affichClassement (const vector<participant> & vPart)
{
    unsigned rang (1); //Le rang commence a 1 et ne peut pas être négatif
    for (const participant & part : vPart)
    {
        cout << rang << ":" << part.prenom << endl; //affichage
        rang = rang + 1; //on incrémente de 1 pour le prochain participant
    }
}

int pfc(int choixA, int choixB) //return 1 si A gagne, 2 si B gagne, 0 si égalité
{
    if (choixA == choixB) return 0; //si égalité on return 0
    if ((choixA == 0 && choixB == 2) || //pierre bat ciseaux
        (choixA == 1 && choixB == 0) ||
        (choixA == 2 && choixB == 1))
        return 1; //A gagne
    return 2; //sinon B gagne
}

int pof(int tirageA, int tirageB) //si exclusivement une personne tombe sur pile elle gagne
{
    if (tirageA == 0 && tirageB == 1) return 1; //le joueur A tombe sur pile et B sur face
    if (tirageB == 0 && tirageA == 1) return 2;
    return 0; //si égalité on return 0
}

int des(int faceA, int faceB) //la personne avec le plus grand chiffre gagne
{
    if (faceA > faceB) return 1;
    if (faceB > faceA) return 2;
    return 0; //si égalité on return 0
}

int main()
{
    vector<participant> vParticipant; //la liste de tous les participants
    vector<int> vActions; //on declare le tableau des actions des joueurs (int car les valeurs sont des int entre 0 et 6 en fonction du jeu)

    unsigned n (litUnEntier()); //lit le nombre de joueurs
    for (unsigned i (0); i < n; i = i + 1) //boucle pour lire chaque joueur
    {
        string prenom (litUneString()); //lit le prénom du joueur
        vParticipant.push_back(participant{prenom, 0}); //on ajoute le joueur à la liste avec 0 point
    }

    unsigned nbActions (litUnEntier()); //lit le nombre d'actions
    for (unsigned i (0); i < nbActions; i = i + 1) //boucle pour lire chaque action
    {
        int action (litUnEntier()); //lit l'action sous forme d'entier
        vActions.push_back(action); //on ajoute l'action à la liste
    }

    unsigned k (0); //indice de départ pour le tableau vActions
    for (unsigned jeu (1); jeu <= 3; jeu = jeu + 1) //on parcours les 3 jeux
    {
        for (unsigned i (0); i < n; i = i + 1) //on prends le premier joueur pour le Xème match
        {
            for (unsigned j (i+1); j < n; j = j + 1) //j commence à i+1 comme ça chaque match est joué une seule fois, on peut pas jouer contre soi meme donc on commence a i+1 (i qui est nous même)
            {
                int resultat (0); //1 si i gagne, 2 si j gagne, 0 si égalité

                while ((resultat == 0) && (k + 2 <= nbActions)) //on rejoue tant qu'il y a pas de gagnant
                {
                    if (jeu == 1) //on commence par pfc
                        resultat = pfc (vActions[k], vActions[k+1]); //appelle la fonction et renvoie un entier pour savoir qui gagne
                    else if (jeu == 2)
                        resultat = pof (vActions[k], vActions[k+1]);
                    else //sinon c'est des
                        resultat = des (vActions[k], vActions[k+1]);
                    k = k + 2; //apres chaque match on va a la prochaine case libre, pour le 2eme match c'est la 3eme case du tableau
                }

                if (resultat == 1) vParticipant[i].points = vParticipant[i].points + 1; //i gagne 1 point
                if (resultat == 2) vParticipant[j].points = vParticipant[j].points + 1;
            }
        }
    }

    sort (vParticipant.begin(), vParticipant.end(), compare2part); //trie les participants par points
    affichClassement (vParticipant); //affiche le classement

    return 0;
}