#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, m, a;
    cin >> m >> n >> a;
    vector<vector<string>> nums(m, vector<string>(n));
    vector<vector<string>> backup ;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            cin >> nums[i][j];
        }
    }
    backup = nums;
    vector<string> tmp = nums.back();
    for (int i = m - 1; i >= 1; i--) {
        nums[i] = nums[i - 1];
    }
    nums[0] = tmp;
    tmp = vector<string>(n);
    for (int i = 0; i < m; i++) tmp[i] = nums[i][n - 1];
    for (int j = n - 1; j >= 1; j--) {
        for (int i = 0; i < m - 1; i++) nums[i][j] = nums[i][j - 1];
    }
    for (int i = 0; i < m; i++) nums[i][0] = tmp[i];
    long long ans = 0;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < a; k++) {
                ans += backup[i][j][k] != nums[i][j][k];
            }
        }
    }
    cout << ans << endl;
    system("pause");
    return 0;
}
/*
3 3 2
01 10 00
10 00 11
01 00 00
*/