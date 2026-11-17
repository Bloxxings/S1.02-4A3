# Système suisse — trois jeux

Ce projet contient un seul programme en C++17. Il lit un ou plusieurs blocs de tournoi depuis un fichier d'entrée, calcule les appariements et le classement, puis écrit les classements dans l'ordre des blocs. Il n'y a pas de saisie interactive pendant l'exécution.

Chaque bloc commence par une ligne `Jeu:` suivie de `PFC`, `Des` ou `PileFace`. Les joueurs et leurs actions sont déjà indiqués dans le fichier. Pour Pile ou Face, le résultat de chaque lancer est aussi fourni avec `Resultat:`. Les noms utilisés sont Adam O, Adam K, Tony et Ilyess.

Le tournoi comporte trois rondes. Les joueurs ayant un score proche sont appariés en priorité et les revanches sont évitées si possible. Une victoire vaut 3 points et une égalité vaut 1 point à chaque joueur. Le classement est trié par points, puis par victoires.

## Organisation des tests

Le dossier `tests/` contient deux tests complets. Chaque entrée regroupe un scénario de chacun des trois jeux, dans l'ordre PFC, dés, puis pile ou face. L'oracle correspondant regroupe les trois classements attendus dans le même ordre. Les fichiers `resultat1.out` et `resultat2.out` reçoivent la sortie du programme.

- `entree1.txt` : scénarios avec des victoires et des égalités.
- `entree2.txt` : scénarios principalement composés de matchs nuls ou de choix identiques.
- `oracle1.txt` et `oracle2.txt` : classements attendus pour les trois blocs de chaque entrée.

## Compiler et vérifier

Dans Qt Creator, ouvrir `suisse.pro` et compiler le projet. Pour lancer les deux tests et comparer automatiquement les sorties aux oracles, ouvrir le terminal dans le dossier du projet et exécuter :

```sh
./tests/run_tests.sh
```

Le script compile le programme, remplit les deux fichiers `resultat*.out`, puis lance `diff` avec chaque oracle. Un message `OK` indique que le résultat correspond.
