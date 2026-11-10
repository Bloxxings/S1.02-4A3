# Mon système suisse

Dans ce dossier, j’ai organisé trois mini-jeux pour le système de tournoi suisse : pierre-feuille-ciseaux, dés et pile ou face. Chaque jeu a son propre projet Qt Creator et son dossier `tests`.

Pour que les essais donnent toujours le même résultat, je lis les actions depuis les fichiers d’entrée. Pour le jeu de dés, les valeurs du dé sont écrites dans le fichier. Pour pile ou face, les choix des joueurs et le résultat du lancer sont écrits dans le fichier. Les programmes n’utilisent donc pas de tirage aléatoire pendant les tests.
