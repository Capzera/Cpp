#include <bits/stdc++.h>
using namespace std;
int main() {
    int x1, y1, x2, y2;
    cin >> x1 >> y1 >> x2 >> y2;
    int x = abs(x2 - x1), y = abs(y2 - y1);
    cout << __gcd(x, y) + 1 << endl;
    system("pause");
    return 0;
}