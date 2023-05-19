#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, q;
    cin >> n >> q;
    vector<vector<int>> W(n, vector<int>(n)), L(n, vector<int>(n));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            cin >> W[i][j];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            cin >> L[i][j];
    auto check = [&](int x, vector<vector<int>> w) -> int {
        int ans = 0;
        for (int i = 0; i < n; i++) {
            int val = x / n + (x % n > i);
            for (int j = 0; j < n; j++) {
                w[i][j] -= val, w[j][i] -= val;
                w[i][j] = max(w[i][j], L[i][j]);
            }
        }
        for (int k = 0; k < n; k++)
            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++)
                    w[i][j] = min(w[i][j], w[i][k] + w[k][j]);
        for (int i = 0; i < n; i++)
            ans += accumulate(w[i].begin(), w[i].end(), 0);
        return ans;
    };
    int l = 0, r = 1e9;
    while (l < r) {
        int mid = l + (r - l) / 2;
        if (check(mid, W) <= q) {
            r = mid;
        }
        else l = mid + 1;
    }
    cout << (r == 1e9 ? -1 : r) << endl;
    system("pause");
    return 0;
}