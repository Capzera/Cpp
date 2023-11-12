#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, s, m;
    cin >> n >> s >> m;
    double single = 10000000.0 / n;
    int a = n - m, b, c;
    for (b = 0; b <= a; b++) {
        for (c = 0; c <= m; c++) {
            double tot = a * single + b + 0.5 * c * single;
            if (tot >= s && tot - s < 1) {
                cout << "p" << a << "(+" << b << ") f" << c << " l" << m - c << endl;
                return 0;
            }
        }
    }
}