Double bracket

Le code est exécuté en injectant un fichier entrée et en donnant le résultat du programme dans un fichier sortie.
Le fichier entrée contient toutes les saisies normalement réalisées au clavier
En commençant par lister tous les 64 participants. Puis tous les scénarios des matchs sont déjà écrits, le code a simplement à faire en sorte de trouver le bon gagnant du tournoi double bracket.
A la fin de l'exécution, on vérifie si notre fichier sortie est le même que le fichier oracle (qui contient le vainqueur du tournoi), avec la commande "diff".


Fonctionnement du code:
    Le code commence par ajouter tous les participants dans le winner bracket ( parce que personne n'a de défaites pour l'instant). Ensuite on effectur le premier bracket. Chaque match détermine le vainqu(entre les deux joueurs manquants, dont l'un d'entre eux est dans le winner bracket et l'autre est dans le loser bracket).eur grâce à un BO3 
    (Best of 3). 
    Dans le BO3, on fait jouer 3 jeux différents aux concurrents. On commence par un pierre feuille ciseau, puis un jeu de lancé de dés (le plus grand nombre l'emporte). Si un des joueurs a gagné les deux premiers matchs, pas besoin de faire le 3ème jeu, sinon le 3ème jeu est un jeu de cartes (chaque joueur tire une carte et la plus grande carte l'emporte).
    
    Le vainqueur ne bouge pas mais le perdant se déplace dans le loser bracket. 
    Ensuite on effectue les manches du loser bracket, avec le même système de match en BO3, le vainqueur reste dans le loser bracket et le perdant est éliminé du tournoi.
    On répète ces opérations jusqu'à ce qu'il ne reste plus qu'un joueur dans chaque bracket pour la grande finale.
    Enfin, on insère le vainqueur de la grande finale dans le fichier sortie.


Explication des fonctions utilisées:

litUneString():
    Lit le fichier entrée, ligne par ligne (chaque getline() = ligne suivante).
    On vérifie si la ligne n'est pas vide ou si on ne trouve pas d'espace ou de retour à la ligne ou de retour au début de ligne (avec des "\r" et "\t")
    On vzrifie aussi si ce n'est pas un commentaire écrit avec "//"
    Après avoir vérifié tout ça, on peut enfin return la ligne.

litUnEntier():
    utilise stoi() pour transformer un string (obtenu par la litUneString()) en entier

pfc():
    fonction qui simule un pierre feuille ciseau,
    on commence par obtenir les choix des joueurs.
    on choisit 0, 1 ou 2, qui correspondent à 0(pierre) 1(feuille) et 2(ciseau)
    La pierre mange le ciseau, le ciseau mange la feuille et la feuille mange la pierre
    (0 == (2+1)%3) ou (1 == (0+1)%3) ou (2 == (1+1)%3)
    On retourne 0 si on obtient une égalité, 1 si le joueur 1 gagne, 2 si le joueur 2 gagne

des():
    fonction qui simule un jeu de dés,
    on commence par obtenir les choix des joueurs.
    Et en return le joueur qui obtient le plus grand nombre, de la même manière que la fonction pfc()

carte():
    fonction qui simule un jeu de carte,
    on commence par obtenir les choix des joueurs.
    Et en return le joueur qui obtient la plus grande carte, de la même manière que la fonction pfc() et des()

jouerBO3():
    Dans le BO3, on fait jouer 3 jeux différents aux concurrents. On commence par un pierre feuille ciseau, puis un jeu de lancé de dés (le plus grand nombre l'emporte). Si un des joueurs a gagné les deux premiers matchs, pas besoin de faire le 3ème jeu, sinon le 3ème jeu est un jeu de cartes.
    A la fin, on return le vainqueur du round BO3

tourWinnerBracket():
    on initialise une liste vide qui va stocker les gagnants du bracket
    on parcourt le winner bracket par itération de 2 (pour prendre 2 joueurs à la fois)
    Ensuite on fait un round de BO3. Le vainqueur est ajouté à la liste des gagnats du bracket et le perdant se déplace du winner bracket vers le loser bracket.
    Enfin, notre winner bracket devient la liste des gagnants du bracket.

tourLoserBracket():
    on initialise une liste vide qui va stocker les gagnants du bracket
    on parcourt le loser bracket par itération de 2 (pour prendre 2 joueurs à la fois)
    Ensuite on fait un round de BO3. Le vainqueur est ajouté à la liste des gagnats du bracket et le perdant est éliminé du tournoi.
    Enfin, notre loser bracket devient la liste des gagnants du bracket.

main():
    on initialise le winner bracket et le loser bracket
    Ensuite, on ajoute tous les participants dans le winner bracket

    Ensuite, comme 2⁶ = 64, on effectue 6 fois la boucle suivante:
    On appelle notre fonction qui fait jouer le winner bracket. Si la taille de loser bracket est supérieure à 2, on n'est pas encore en finale, donc on continue le programme en faisant un tour du loser bracket.
    A la fin de cette boucle, on vide le loser bracket jusqu'à ce qu'il reste une seule personne.
    Une fois qu'il reste un seul joueur dans chaque bracket, on sait que c'est la grande finale.
    Pour la jouer, on appelle une dernière fois notre fonction BO3.
    Enfin, on ajoute au fichier de sortie le vainqueur du tournoi.ssss