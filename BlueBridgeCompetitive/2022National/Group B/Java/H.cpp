#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, m, d;
    cin >> m >> n >> d;
    vector<int> a(m + 1), b(n + 1);
    for (int i = 1; i <= m; i++) cin >> a[i];
    for (int j = 1; j <= n; j++) cin >> b[j];
    sort(a.begin(), a.end()), sort(b.begin(), b.end());
    vector<vector<vector<double> > > dp(m + 1, vector<vector<double> >(n + 1, vector<double>(2)));
    dp[1][0][0] = a[1];
    dp[0][1][1] = sqrt(1.0 * b[1] * b[1] + d * d);
    for (int i = 1; i <= m; i++)
        dp[i][0] = {dp[1][0][0] + a[i] - a[1], DBL_MAX};
    for (int j = 1; j <= n; j++)
        dp[0][j] = {DBL_MAX, dp[0][1][1] + b[j] - b[1]};
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            double dis = sqrt(1.0 * (a[i] - b[j]) * (a[i] - b[j]) + 1.0 * d * d);
            dp[i][j][0] = min(dp[i - 1][j][0] + a[i] - a[i - 1], dp[i - 1][j][1] + dis);
            dp[i][j][1] = min(dp[i][j - 1][1] + b[j] - b[j - 1], dp[i][j - 1][0] + dis);
        }
    }
    cout << fixed << setprecision(2) << min(dp[m][n][0], dp[m][n][1]) << endl;
    system("pause");
    return 0;
}