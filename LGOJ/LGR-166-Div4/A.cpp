#include <bits/stdc++.h>
using namespace std;
int main() {
    int a, b, r, v, m;
    cin >> a >> b >> r >> v >> m;
    int rice, vege, meat;
    cout << 2 * a * r * 2 + b * r << " " << 3 * a * v * 2 + b * v << " " << 3 * a * m * 2 + b * m << endl;
    system("pause");
    return 0;
}