#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, m, i, j, k, op;
    cin >> n >> m;
    vector<unordered_map<int, int> > mp(n + 1);
    while (m--) {
        cin >> op;
        if (op == 1) {
            cin >> i >> j >> k;
            mp[i][j] = k;
        }
        if (op == 2) {
            cin >> i >> j;
            cout << mp[i][j] << endl;
        }
    }
    system("pause");
    return 0;
}