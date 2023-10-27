#include <bits/stdc++.h>
using namespace std;
int main() {
    int m, n;
    cin >> m >> n;
    vector<vector<int> > ans(n, vector<int>(m));
    for (int j = 0; j < m; j++)
        for (int i = 0; i < n; i++)
            cin >> ans[i][j];
    for (auto& i : ans) {
        for (auto& j : i) cout << j << " ";
        cout << endl;
    }
    system("pause");
    return 0;
}