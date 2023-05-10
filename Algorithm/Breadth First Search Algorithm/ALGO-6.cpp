#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, m, ans = INT_MAX;
    cin >> n >> m;
    vector<int> c(n + 1), fa(n + 1);
    vector<vector<int> > edge(m, vector<int>(3));
    for (int i = 1; i <= n; i++) {
        cin >> c[i];
        ans = min(ans, c[i]);
        fa[i] = i;
    }
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < 3; j++) cin >> edge[i][j];
        edge[i][2] = 2 * edge[i][2] + c[edge[i][0]] + c[edge[i][1]];
    }
    sort(edge.begin(), edge.end(), [](auto& a, auto& b) {
        return a[2] < b[2];
    });
    for (int i = 0; i < m; i++) {
        int n1 = fa[edge[i][0]], n2 = fa[edge[i][1]];
        if (n1 == n2) continue;
        ans += edge[i][2];
        for (int j = 1; j <= n; j++) {
            if (fa[j] == n2)
                fa[j] = n1;
        }
    }
    cout << ans << endl;
    system("pause");
    return 0;
}