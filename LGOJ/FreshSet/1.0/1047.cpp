#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, m, u, v, ans = 0;
    cin >> m >> n;
    vector<int> tree(m + 1);
    for (int i = 0; i < n; i++) {
        cin >> u >> v;
        for (int j = u; j <= v; j++) tree[j] = 1;
    }
    cout << m - accumulate(tree.begin(), tree.end(), 0) + 1 << endl;
    system("pause");
    return 0;
}