# SAÉ 1.02 : Tester et comparer des algorithmes

## Contexte pédagogique (R1.01)

Dans le cadre de cette SAÉ, vous allez concevoir le cœur algorithmique d’une plateforme d’organisation de tournois. L’objectif est de programmer un système capable d’enregistrer des joueurs, de les faire s’affronter sur un jeu simple, et de déterminer un classement final selon un format de compétition précis.

Pour garantir la fiabilité de votre programme, vous devrez valider votre code grâce à un système de tests automatisés basés sur des fichiers oracles et la redirection de flux sous Linux.

---

## 1. L’Algorithme : Mini-jeu et Format de Tournoi

Vous devez concevoir et implémenter deux éléments principaux interdépendants :

* **Un mini-jeu :** Vous programmerez un jeu simple permettant de départager deux joueurs (par exemple un jeu de “Pile ou Face”, un “Pierre-Feuille-Ciseaux”, un jeu de dés, un pendu, etc.).
* **Un système de tournoi :** Vous devrez programmer l’organisation d’une compétition complète en implémentant les méthodes d’appariement suivantes :
* Le Système Suisse
* Le Round Robin (Toutes rondes)
* Le Double Bracket (Arbre à élimination avec Loser Bracket)



> **Note :** Le programme devra faire s’affronter les joueurs inscrits selon le format de tournoi choisi, en faisant appel au mini-jeu pour déterminer le vainqueur de chaque match. Pour que vos tests automatisés fonctionnent, les actions ou résultats aléatoires devront être lus depuis l’entrée standard.

---

## 2. Validation par Oracles et Redirection de flux

Votre programme ne doit pas ouvrir ou fermer de fichiers directement dans le code. Il doit lire les informations depuis l’entrée standard (le clavier) et afficher le résultat sur la sortie standard (l’écran).

Pour automatiser les tests avec vos fichiers d’essais, vous utiliserez la redirection de flux sous Linux (`<` et `>`).

Si votre exécutable s’appelle `moteur_tournoi`, vous le testerez ainsi dans le terminal :

```bash
./moteur_tournoi < test1.in > resultat1.out

```

Ensuite, vous vérifierez que votre sortie correspond exactement à l’oracle attendu en utilisant la commande `diff` :

```bash
diff resultat1.out test1_oracle.out

```

*(Si la commande `diff` n’affiche rien, c’est que votre programme fonctionne parfaitement !)*

### Spécification des entrées attendues (Gestion des commentaires)

Votre programme lira la liste des participants, puis les actions nécessaires au déroulement des mini-jeux de tout le tournoi.

> **Attention :** Les fichiers d’entrée peuvent contenir des lignes de commentaires pour documenter les tests. Ces lignes commencent toujours par le caractère `#`. Votre programme doit être capable d’ignorer ces lignes lors de la lecture.

**Exemple de données lues sur l’entrée standard :**

```text
# Liste des participants
Joueur:Alice
Joueur:Bob
Joueur:Charlie
Joueur:David
# Début du tournoi (ex: Pile=0, Face=1 pour les actions des joueurs)
Action:0
Action:1
Action:1
Action:1

```

### Spécification des sorties attendues sur la sortie standard

Votre programme devra afficher exactement le classement final du tournoi, sans aucun texte superflu ni commentaire de débogage :

```text
1:Charlie
2:Alice
3:Bob
4:David

```

---

## 3. Jeux d’essais (Fichiers fournis pour vos tests)

Pour valider votre code, vous devez tester au minimum deux scénarios (oracles) complets de tournois. Assurez-vous que votre programme gère bien les lignes de commentaires présentes dans les fichiers `.in`.

### Oracle 1 : Tournoi classique (4 joueurs)

* **Fichier d’entrée (`test1.in`) :**
```text
# Inscription des 4 joueurs
Joueur:J1
Joueur:J2
Joueur:J3
Joueur:J4
# Série d'actions pré-définies pour valider la logique de votre format de tournoi
Action:0
Action:1
Action:2
Action:0

```


* **Fichier Oracle (`test1_oracle.out`) :**
```text
1:J3
2:J1
3:J4
4:J2

```



### Oracle 2 : Tournoi avec égalités ou repêchages

* **Fichier d’entrée (`test2.in`) :**
```text
# Inscription
Joueur:A
Joueur:B
Joueur:C
Joueur:D
# Actions déclenchant des scénarios particuliers (matchs nuls, loser bracket, etc.)
Action:1
Action:1
Action:0
Action:1
Action:2

```


* **Fichier Oracle (`test2_oracle.out`) :**
```text
1:A
2:C
3:B
4:D

```



---

## 4. Travail demandé (Livrables)

Le rendu de votre SAÉ se fera selon les modalités suivantes :

* **Date limite :** Rendu avant le 6 novembre (18h00) sur la plateforme **Ametice**.
* **Format de l’archive :** Une archive au format `.zip` ne contenant **aucun binaire / exécutable** (ni `.o`).
* **Contenu de l'archive :**
* Un PDF expliquant quels systèmes de classement vous avez codé.
* Au moins 3 projets (sous **Qt Creator**) – un projet par système de classement.
* Au moins 2 jeux d’essais (par projet) vérifiant vos propositions.
* Pour cela, on va utiliser des fichiers oracles ([en savoir plus](https://en.wikipedia.org/wiki/Test_oracle)). Cela implique au moins 2 jeux d’essais et 2 fichiers oracles relatifs par projet.



### Nomenclature de l’archive

Votre archive devra suivre la nomenclature suivante :
`TD_AA_E_BB_YYYY_ZZZ_BBBB_CCCC.zip`

Dans cette nomenclature :

* `AA` représente votre groupe de TD.
* `BB` représente votre numéro d’équipe.
* `YYYY`, `BBBB`, `CCCC`, etc., représentent les noms des étudiants de votre équipe, triés par ordre alphabétique.

---

## 5. Modalités de présentation et d’évaluation

* **Présentation :** Prévoir une présentation d'environ 10 minutes lors de la semaine du 16 novembre.
* **Barème (sur 30 points) :**
* **Rapport :** 2 pts
* **Présentation / diaporama (sans code) :** 3 pts
* **Systèmes de classement :** 5 pts par système (soit 15 pts au total), répartis comme suit :
* 4 pts par système de classement (codage d’un algorithme).
* 1 pt par fichier oracle pour un même système de vote.


* **Évaluation :** 2 pts par système de classement corrigé par les pairs.
* **Points de répartition libre :**
* Chaque étudiant de chaque groupe de projet dispose de **4 pts** qu’il peut répartir comme il le souhaite.
* Par conséquent, une équipe de 4 étudiants dispose de 16 points à se répartir (20 points pour une équipe de 5 étudiants).
* L’équipe pédagogique n’interviendra pas dans la répartition de ces points, sauf si un ou plusieurs étudiants obtiennent une note $> 30$.





### Pénalités

* **Fautes d’orthographe :** Si plus de 2 fautes par page / par slide $\rightarrow$ **$-1$ pt**.
* **Bugs découverts :**
* Premier bug : **$-3$ pts**
* Second bug : **$-2$ pts**
* Troisième bug : **$-1$ pt** *(Plafond des bugs : à partir de 3 bugs, le malus total est de $-6$ pts, les tests s'arrêtent là).*


* **Triche / Plagiat :**
* **Copie sur un autre groupe :** Note de $0$ + passage en conseil de discipline.
* **Intégration de code provenant d’un tiers (développeur externe, forum, LLM, etc.) :**
* *Si vous ne sourcez pas :* assimilé à de la copie (voir ci-dessus).
* *Si vous sourcez :*
* Note divisée par 2 si vous êtes capables d’expliquer le code.
* Note de $0$ sinon.