#!/bin/sh
set -eu
cd "$(dirname "$0")/.."
mkdir -p build
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o build/suisse
for numero in 1 2; do
    ./build/suisse < "tests/entree${numero}.txt" > "tests/resultat${numero}.out"
    if diff -u "tests/oracle${numero}.txt" "tests/resultat${numero}.out"; then
        echo "OK : test ${numero}"
    else
        echo "ÉCHEC : test ${numero}"
        exit 1
    fi
done
