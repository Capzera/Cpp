#include <bits/stdc++.h>
using namespace std;
const int N = 9;
int main() {
    long long ans = 0;
    vector<int> grid(81), mi = {0, 3, 6, 27, 30, 33, 54, 57, 60};
    auto check = [&](int idx, int num) -> bool {
        int row = idx / 9, col = idx % 9, miya = row / 3 * 3 + col / 3;
        //检查行
        for (int i = row * 9; i < idx; i++) {
            if (grid[i] == num) return 0;
        }
        //检查列
        for (int j = col; j < idx; j += 9) {
            if (grid[j] == num) return 0;
        }
        for (int k = mi[miya], cnt = 0; k < idx; k++, cnt++) {
            if (grid[k] == num) return 0;
            if (cnt == 2) {
                cnt = -1;
                k += 6;
            }
        }
        return 1;
    };
    function<void(int)> dfs = [&](int idx) -> void {
        if (idx == 81) {
            ans++;
            return;
        }
        for (int i = 1; i <= 9; i++) {
            if (!check(idx, i)) continue;
            dfs(idx + 1);
        }
    };
    dfs(0);
    cout << ans << endl;
    system("pause");
    return 0;
}