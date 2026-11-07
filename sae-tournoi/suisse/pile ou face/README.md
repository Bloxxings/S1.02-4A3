# Mon jeu de pile ou face

Le code du jeu se trouve dans `main.cpp`. Le projet à ouvrir dans Qt Creator est `pile_ou_face.pro`.

Pour chaque match, les deux joueurs annoncent `Pile` ou `Face`, puis le fichier d’entrée donne le résultat du lancer avec une ligne `Resultat: Pile` ou `Resultat: Face`. Ces résultats sont fixés dans le fichier : le programme ne lance pas de pièce au hasard. Si un seul joueur a deviné le résultat, il gagne. Si les deux ont la même justesse (tous les deux ont deviné juste ou tous les deux ont deviné faux), le match est nul. Une victoire rapporte 3 points; une égalité rapporte 1 point à chaque joueur.

Dans `tests/`, j’ai préparé trois tournois avec leur classement attendu (`oracleN.txt`) et le fichier où j’enregistre la sortie (`resultatN.out`). J’ai aussi ajouté deux entrées pour vérifier des erreurs : un choix invalide et un nombre impair de joueurs.

## Compiler et vérifier

J’ouvre `pile_ou_face.pro` dans Qt Creator, je sélectionne le kit, puis je compile avec le bouton marteau. Pour compiler et lancer les tests depuis le Terminal, en étant dans ce dossier :

```sh
g++ -std=c++17 main.cpp -o pile_ou_face
./pile_ou_face < tests/entree1.txt > tests/resultat1.out
diff tests/resultat1.out tests/oracle1.txt
```

Je répète les deux dernières commandes avec `2`, puis `3`. Si `diff` n’affiche rien, le classement obtenu correspond à l’oracle. Le classement est enregistré dans `resultatN.out`; les points et les victoires s’affichent dans le Terminal. Pour voir le message d’erreur d’un scénario invalide, je lance par exemple `./pile_ou_face < tests/erreur_action.txt`.
