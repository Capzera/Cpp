#include <bits/stdc++.h>
using namespace std;
int A, B, C;
int solve () {
    int a, b, c, d;
    cin >> a >> b >> c >> d;
    if ((a + c) % 2) return 0;
    A = (a + c) / 2;
    if ((c - a) % 2) return 0;
    B = (c - a) / 2;
    C = d - B;
    if (B - C != b) return 0;
    return 1;
}
int main() {
    if (solve()) cout << A << " " << B << " " << C << endl;
    else cout << "No";
    system("pause");
    return 0;
}