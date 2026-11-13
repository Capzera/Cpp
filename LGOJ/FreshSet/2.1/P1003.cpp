#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, x, y, ans = -2;
    cin >> n;
    vector<vector<int>> mat(n, vector<int>(4));
    for (auto& i : mat) cin >> i[0] >> i[1] >> i[2] >> i[3];
    cin >> x >> y;
    for (int i = 0; i < n; i++) {
        if (mat[i][0] <= x && mat[i][1] <= y && mat[i][0] + mat[i][2] >= x && mat[i][1] + mat[i][3] >= y) ans = i;
    }
    cout << ans + 1 << endl;
    system("pause");
    return 0;
}