# Round-Robin

Chaque joueur affronte tous les autres une seule fois. 1 victoire = 1 point, défaite = 0 point, égalité = on refait le match jusqu'à avoir un gagnant. On fait ensuite un classement par points ; si deux joueurs ont le même score, celui inscrit en premier passe devant, comme au ping-pong.

**Fonctionnement :** le programme lit les entrées données sous fichier txt (joueurs, choix de jeu, actions), en ignorant les lignes qui commencent par '&&' car celles-ci sont des commentaires. Puis une double boucle fait jouer chaque paire une fois, les points sont comptés, triés par ordre décroissant, puis on affiche 1:Nom_joueur1, 2:Nom_joueur2, etc.

**Les 3 mini-jeux :**
- pfc (pierre feuille ciseaux) : pierre-feuille-ciseaux, 0 = pierre, 1 = feuille, 2 = ciseaux, 1 action par joueur par match.
- pof (pile ou face) : le résultat du lancer est lu dans le fichier entrée, 0 = Pile (gagnant si l'autre joueur fais face) , 1 = Face, 1 action par joueur par match.
- des (jeu de dés) : chaque joueur donne un résultat de lancé de dé de 1 à 6, le plus grand gagne, 1 action par joueur par match.

Le jeu ne contient pas de hasard.

**Tests :** il existe deux fichiers d'entrée, cela remplace et permet d'automatiser les entrées utilisateur (noms, nombre de joueurs, choix dans chaque jeu). Pour comparer le fichier oracle et la sortie après exécution du code, on utilise diff comme présenté ci-dessous :
`./round-robin < entree1.txt | diff - oracle1.txt` (rien d'affiché = réussi).

**Source :** code écrit à la main, j'ai seulement utilisé les ressources fournies par M. Casali (CM, Exos, TD, exemples trouvables sur le site casali.site ainsi que sur le dossier code_amphi).