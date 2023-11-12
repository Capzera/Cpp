#include <bits/stdc++.h>
using namespace std;
int main() {
    int m, n;
    cin >> m >> n;
    vector<vector<int> > nums(2 * m, vector<int>(2 * n));
    auto get = [&](int x, int y) -> int {return nums[x][y] + nums[x][y + 1] + nums[x + 1][y] + nums[x + 1][y + 1];};
    for (int i = 0; i < 2 * m; i++)
        for (int j = 0; j < 2 * n; j++) cin >> nums[i][j];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++)
            cout << get(2 * j, 2 * i) << " ";
        cout << endl;
    }
    system("pause");
    return 0;
}