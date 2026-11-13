#include <bits/stdc++.h>
using namespace std;
int main() {
    cin.tie(nullptr)->sync_with_stdio(0);
    int m, n, x;
    cin >> m >> n;
    int nums[n][m];
    memset(nums, 0, sizeof(nums));
    for (int i = 0; i < 2 * m; i++)
        for (int j = 0; j < 2 * n; j++) {
            cin >> x;
            nums[j >> 1][i >> 1] += x;
        }
    for (auto& i : nums) {
        for (auto& j : i) cout << j << " ";
        cout << endl;
    }
    system("pause");
    return 0;
}