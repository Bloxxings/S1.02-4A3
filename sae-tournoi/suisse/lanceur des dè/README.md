# Mon jeu de dés

Le code du jeu se trouve dans `main.cpp`. Le projet à ouvrir dans Qt Creator est `lanceur_des_des.pro`.

Pour chaque match, le fichier d’entrée donne le résultat du dé de chaque joueur avec une ligne comme `Action de Alice: 5`. Les valeurs possibles vont de 1 à 6. Le plus grand résultat gagne le match; si les deux résultats sont égaux, le match est nul. Les points du tournoi sont les mêmes que pour mon autre jeu : 3 points pour une victoire, 1 point chacun en cas d’égalité, 0 point pour une défaite. Les valeurs sont lues dans l’entrée, donc le programme ne lance pas de dé au hasard.

Dans `tests/`, j’ai préparé trois tournois avec leur classement attendu (`oracleN.txt`) et le fichier où j’enregistre la sortie (`resultatN.out`). J’ai aussi ajouté deux entrées pour vérifier des erreurs : un résultat de dé invalide et un nombre impair de joueurs.

## Compiler et vérifier

J’ouvre `lanceur_des_des.pro` dans Qt Creator, je sélectionne le kit, puis je compile avec le bouton marteau. Pour compiler et lancer les tests depuis le Terminal, en étant dans ce dossier :

```sh
g++ -std=c++17 main.cpp -o lanceur_des_des
./lanceur_des_des < tests/entree1.txt > tests/resultat1.out
diff tests/resultat1.out tests/oracle1.txt
```

Je répète les deux dernières commandes avec `2`, puis `3`. Si `diff` n’affiche rien, le classement obtenu correspond à l’oracle. Le classement est enregistré dans `resultatN.out`; les points et les victoires s’affichent dans le Terminal. Pour voir le message d’erreur d’un scénario invalide, je lance par exemple `./lanceur_des_des < tests/erreur_action.txt`.
