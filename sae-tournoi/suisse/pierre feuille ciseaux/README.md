# Pierre, feuille, ciseaux

Le projet C++ est dans `main.cpp` et le projet Qt Creator dans `suisse.pro`.

Dans `tests/`, chaque scénario regroupe son entrée (`entreeN.txt`), le classement attendu (`oracleN.txt`) et le résultat produit (`resultatN.out`). Les résultats sont gardés ici comme traces des essais.

Ouvre `suisse.pro` avec Qt Creator pour compiler le programme. Pour vérifier un scénario depuis ce dossier après compilation :

```sh
./build/Qt_6_12_0_for_macOS_Debug/suisse < tests/entree1.txt > tests/resultat1.out
diff tests/resultat1.out tests/oracle1.txt
```

Répète les deux commandes avec `2`, puis `3`, pour les autres scénarios.
