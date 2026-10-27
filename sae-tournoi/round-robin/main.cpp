#include <iostream>
using namespace std;

int main() {
    int n;
    cout << "Entre un nombre : ";
    cin >> n;

    if (n % 2 == 0) {
        cout << n << " est pair" << endl;
    } else {
        cout << n << " est impair" << endl;
    }

    return 0;
}