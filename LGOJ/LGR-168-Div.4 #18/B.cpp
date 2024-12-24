#include <bits/stdc++.h>
using namespace std;
int main() {
    long long x1, x2, y1, y2;
    cin >> x1 >> y1 >> x2 >> y2;
    if (y1 * y2 > INT_MAX || x1 * x2 < INT_MIN) cout << "long long int" << endl;
    else cout << "int" << endl;
    system("pause");
    return 0;
}