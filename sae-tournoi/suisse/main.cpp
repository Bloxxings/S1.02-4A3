#include <iostream>

int main() {
    int a = 4;
    int b = 2;

    if (a > b) {
        std::cout << "Le premier nombre est plus grand.\n";
    } else if (b > a) {
        std::cout << "Le deuxième nombre est plus grand.\n";
    } else {
        std::cout << "Les nombres sont egaux.\n";
    }

    return 0;
}