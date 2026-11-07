# Pierre, feuille, ciseaux

Dans ce dossier, j’ai mis le code de mon jeu dans `main.cpp` et le projet à ouvrir dans Qt Creator dans `suisse.pro`.

Le programme lit les joueurs et leurs actions depuis un fichier d’entrée. Il joue les trois rondes du tournoi suisse, calcule les points et les victoires, puis affiche le classement.

Dans `tests/`, j’ai rangé trois scénarios. Pour chacun, il y a une entrée (`entreeN.txt`), le classement que j’attends (`oracleN.txt`) et le résultat obtenu en lançant le programme (`resultatN.out`). J’ai calculé l’oracle séparément pour pouvoir comparer le résultat du programme avec `diff`.

Pour compiler le programme, j’ouvre `suisse.pro` dans Qt Creator, je sélectionne mon kit et je clique sur le marteau. Après la compilation, je peux lancer un test depuis ce dossier avec le Terminal :

```sh
./build/Qt_6_12_0_for_macOS_Debug/suisse < tests/entree1.txt > tests/resultat1.out
diff tests/resultat1.out tests/oracle1.txt
```

Si `diff` n’affiche rien, le résultat obtenu correspond à l’oracle. Je répète les commandes avec `2`, puis `3`, pour vérifier les autres scénarios.
