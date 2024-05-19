#include <bits/stdc++.h>
using namespace std;
int main() {
    int m, n, cube = 0, rect = 0;
    cin >> m >> n;
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (i == j) cube += (m - i + 1) * (n - j + 1);
            else rect += (m - i + 1) * (n - j + 1);
        }
    }
    cout << cube << " " << rect << endl;
    system("pause");
    return 0;
}