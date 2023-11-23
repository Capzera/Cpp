#include <bits/stdc++.h>
using namespace std;
int main() {
    int d, t, m, ans = 0, mod = 1e9 + 7;
    cin >> d >> t >> m;
    function<void(int, int, int)>dfs = [&](int strenth, int loc, int time) -> void {
        if (!loc) return;
        if (!time) {
            ans++;
            return;
        }
        if (strenth) dfs(strenth - 1, loc + 1, time - 1);
        dfs(strenth, loc - 1, time - 1);
    };
    dfs(m, d, t);
    cout << ans << endl;
    system("pause");
    return 0;
}