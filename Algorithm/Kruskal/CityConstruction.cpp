#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, m, ans, port;
    cin >> n >> m;
    vector<int> fa(n + 1);
    vector<vector<int>> edge(m, vector<int>(3));
    for (int i = 0; i < m; i++)
        for (int j = 0; j < 3; j++) 
            cin >> edge[i][j];
    auto Kruskal = [&](int st) -> int {
        int ans = 0;
        sort(edge.begin(), edge.end(), [&](auto& a, auto& b) {
            return a[2] < b[2];
        });
        iota(fa.begin(), fa.end(), 0);
        for (auto& e : edge) {
            int n1 = fa[e[0]], n2 = fa[e[1]];
            if (n1 == n2) continue;
            ans += e[2];
            for (int j = st; j <= n; j++) {
                if (fa[j] == n2) fa[j] = n1;
            }
        }
        return ans;
    };
    ans = Kruskal(1);
    for (int i = 1; i <= n; i++) {
        cin >> port;
        if (port != -1) edge.push_back({0, i, port});
    }
    ans = min(Kruskal(0), ans);
    cout << ans << endl;
    system("pause");
    return 0;
}