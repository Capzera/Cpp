#include <bits/stdc++.h>
using namespace std;
int main() {
    int a, b, c, d, m;
    cin >> a >> b >> c >> d >> m;
    cout << max(0, c * 5 + 20 * d * (c * 5 > m) - 20 * (b - d)) << endl;
    system("pause");
    return 0;
}