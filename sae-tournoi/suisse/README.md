# Système suisse

Ce dossier contient un seul projet Qt Creator : `suisse.pro`. Le programme lit le tournoi depuis l'entrée standard et écrit le classement final sur la sortie standard. Il ne lance pas de partie interactive et ne fait aucun tirage aléatoire.

Chaque entrée commence par `Jeu:PFC`, `Jeu:Des` ou `Jeu:PileFace`. Les choix et les résultats nécessaires sont déjà écrits dans le fichier. Les scénarios utilisent les noms Adam O, Adam K, Tony et Ilyess.

Le tournoi comporte trois rondes. Les joueurs ayant un score proche sont appariés en priorité, et les revanches sont évitées si possible. Une victoire vaut 3 points, une égalité 1 point à chaque joueur. Le classement est trié par points, puis par victoires.

## Compiler et lancer les tests

Dans Qt Creator, ouvrir `suisse.pro` et compiler. Les entrées et les oracles sont rangés dans `tests/`, par mini-jeu. Un scénario peut être lancé ainsi depuis ce dossier :

```sh
./build/suisse < tests/pfc/entree1.txt > tests/pfc/resultat1.out
diff tests/pfc/resultat1.out tests/pfc/oracle1.txt
```

Le script `tests/run_tests.sh` compile dans `build/`, lance les neuf scénarios et compare les résultats aux oracles :

```sh
./tests/run_tests.sh
```

Si `diff` ne signale aucune différence, le classement calculé correspond à l'oracle.
