# Système suisse — classement sur trois jeux

Ce projet contient un programme C++ pour organiser un tournoi selon le système suisse. Le fichier d'entrée contient les joueurs et leurs actions pour les trois jeux : pierre-feuille-ciseaux, dés et pile ou face. Le programme calcule séparément le classement de chaque jeu, additionne ensuite les points obtenus dans les trois jeux et affiche le classement général.

Il n'y a pas de saisie interactive pendant l'exécution. Les choix des joueurs et le résultat de chaque lancer de pièce sont écrits dans le fichier d'entrée. Chaque fichier contient exactement un bloc `Jeu:PFC`, un bloc `Jeu:Des` et un bloc `Jeu:PileFace`. Les mêmes joueurs participent aux trois jeux.

Le tournoi de chaque jeu comporte trois rondes. Une victoire vaut 3 points et une égalité vaut 1 point à chaque joueur. Les joueurs ayant un score proche sont appariés en priorité et les revanches sont évitées si possible. Le classement général additionne les points des trois jeux et affiche le total obtenu par chaque joueur. En cas d'égalité de points, les victoires cumulées servent de départage.

## Tests

Le dossier `tests/` contient deux entrées complètes (`entree1.txt`, `entree2.txt`) et leurs oracles (`oracle1.txt`, `oracle2.txt`). Chaque oracle vérifie les classements des trois jeux et le classement général. Les sorties calculées sont enregistrées dans `resultat1.out` et `resultat2.out`.

Pour compiler, exécuter les deux entrées et comparer les résultats aux oracles, lancer depuis le dossier du projet :

```sh
./tests/run_tests.sh
```
Affiche comme resultat :
OK : test 1
OK : test 2

Ensuite dans le Terminal lancer :

```sh
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o build/suisse

./build/suisse < tests/entree1.txt > tests/resultat1.out
diff -u tests/oracle1.txt tests/resultat1.out


./build/suisse < tests/entree2.txt > tests/resultat2.out
diff -u tests/oracle2.txt tests/resultat2.out
```

Si on veut accéder au résultat des classements, on lance :

```sh
cat tests/resultat1.out

cat tests/resultat2.out
```