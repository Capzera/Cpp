#include <bits/stdc++.h>

using namespace std;

int main(){
    int m, n;
    cin >> m >> n;
    vector<vector<int>> nums(m, vector<int> (n)), sum(m + 1, vector<int> (n + 1));
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            cin >> nums[i][j];
            sum[i + 1][j + 1] = sum[i][j + 1] + sum[i + 1][j] - sum[i][j] + nums[i][j];
        }
    }
    for (int i = 0; i <= m; i++) {

        for (int j = 0; j <= n; j++) {
            cout << sum[i][j] << " ";
        }
        cout<<endl;
    }
    //sum[r2 + 1][c2 + 1] - sum[r1][c2 + 1] - sum[r2 + 1][c1] + sum[r1][c1]
    system("pause");
    return 0;
}