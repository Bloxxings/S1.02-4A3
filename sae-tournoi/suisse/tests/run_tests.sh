#!/bin/sh
set -eu
cd "$(dirname "$0")/.."
mkdir -p build
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o build/suisse
for jeu in pfc des pile_face; do
    for numero in 1 2 3; do
        entree="tests/$jeu/entree$numero.txt"
        oracle="tests/$jeu/oracle$numero.txt"
        resultat="tests/$jeu/resultat$numero.out"
        ./build/suisse < "$entree" > "$resultat"
        if diff -u "$oracle" "$resultat"; then
            echo "OK : $jeu scénario $numero"
        else
            echo "ÉCHEC : $jeu scénario $numero"
            exit 1
        fi
    done
done
