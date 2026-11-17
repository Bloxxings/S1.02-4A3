# Tournoi suisse

Le projet Qt Creator à ouvrir est `suisse.pro`. Il construit un seul programme, `suisse`, qui propose trois mini-jeux : pierre-feuille-ciseaux, lancer de dés et pile ou face.

Le tournoi comporte trois rondes. Les joueurs sont appariés en fonction de leurs points et le programme évite si possible les revanches. Une victoire rapporte 3 points, une égalité rapporte 1 point à chaque joueur. Le classement final utilise les points, puis le nombre de victoires.

## Jouer

Dans Qt Creator, ouvrir `suisse.pro`, compiler le projet et le lancer dans un terminal. Si Qt Creator ne permet pas de saisir les réponses, utiliser l'option `--jouer` dans les arguments d'exécution.

Dans le Terminal, depuis ce dossier :

```sh
g++ -std=c++17 main.cpp -o suisse
./suisse --jouer
```

Le programme affiche le menu des jeux, demande les noms des joueurs, puis leurs actions à chaque ronde. Aucun tirage n'est fait par le programme : les valeurs du dé et le résultat de pile ou face sont saisis par les joueurs.

## Vérifier les fichiers de test

Pour tester le jeu de dés sans changer le fichier d'entrée :

```sh
./suisse --jeu des < "lanceur des dè/tests/entree1.txt" > /tmp/resultat.out
diff /tmp/resultat.out "lanceur des dè/tests/oracle1.txt"
```

Pour pile ou face, remplacer `des` par `pile-face` et utiliser les fichiers dans `pile ou face/tests/`. Pour pierre-feuille-ciseaux, utiliser `pfc` et les fichiers dans `pierre feuille ciseaux/tests/`.

Si `diff` n'affiche rien, la sortie du programme correspond à l'oracle du test. Les messages de scores sont envoyés séparément dans le Terminal.
