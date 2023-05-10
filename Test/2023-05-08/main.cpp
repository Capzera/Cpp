#include "test.h"
int main() {
    int a, b;
    cin >> a >> b;
    test T(a, b);
    cout << T.solve() << endl;
    system("pause");
    return 0;
}