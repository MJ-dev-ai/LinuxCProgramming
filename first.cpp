#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
    int a, b, c;
    int x;
    float y;

    cout << "ax^2 + by + c = y\n";

    cout << "Enter a, b, c: ";
    cin >> a >> b >> c;

    cout << "Enter x: ";
    cin >> x;

    y = a*x*x + b*x + c;
    cout << "The answer is " << y << endl;

    return 0;
}