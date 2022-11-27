#include <bits/stdc++.h>
using namespace std;
int main() {
    vector<int> dx = {0, 0, 2, -2}, dy = {2, -2, 0, 0};
    int m, n, ans = 0;
    cin >> m >> n;
    vector<vector<bool> > cake(m, vector<bool> (n, 1));
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (cake[i][j]) {
                ans++;
                for (int d = 0; d < 4; d++) {
                    int mx = i + dx[d], my = j + dy[d];
                    if (mx < 0 || mx >= m || my < 0 || my >= n) continue;
                    cake[mx][my] = 0;
                }
            }
        }
    }
    cout << ans << endl;
    system("pause");
    return 0;
}
/*
#include <bits/stdc++.h>
using namespace std;
int main() {
    int m, n;
    cin >> m >> n;
    if (m % 4 == 2 && n % 4 == 2) cout << m / 2 * n + 2;
    else cout << m * n / 2 + 1;
    return 0;
}
/*
7 * 7 / 2 + 7 % 2 = 25
5 * 6 / 2 + 5 % 2 =
4 * 9 / 2 + 9 % 2 = 19
4 * 11 / 2 + 11 % 2 = 23



6 8
6 * 8 / 2
6 10
6 * 6 / 2 + 2
2 * 2 / 2 + 2
*/
*/