#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, s, m;
    cin >> n >> s >> m;
    double single = 10000000.0 / n;
    for (int b = 0; b <= n - m; b++) {
        for (int c = 0; c <= m; c++) {
            double tot = (n - m) * single + b + 0.5 * c * single;
            if (tot >= s && tot - s < 1) {
                cout << "p" << (n - m) << "(+" << b << ") f" << c << " l" << m - c << endl;
                return 0;
            }
        }
    }
}