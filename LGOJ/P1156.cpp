#include <bits/stdc++.h>
using namespace std;
int main() {
	int d, n, sum = 10;
	cin >> d >> n;
	vector<vector<int> > a(n + 1, vector<int>(3)), dp(n + 1, vector<int>(d + 1, -1));
	for (int i = 1; i <= n; i++)
		for (int j = 0; j < 3; j++)
			cin >> a[i][j];
	sort(a.begin() + 1, a.end());
	dp[0][0] = 10;
	for (int i = 1; i <= n; i++) {
		for (int j = 0; j <= d; j++) {
			if (dp[i - 1][j] < 0) continue;
			int diff = a[i][0] - a[i - 1][0];
			if (j + a[i][2] >= d && dp[i - 1][j] >= diff) {
				cout << a[i][0];
				return 0;
			}
			if (dp[i - 1][j] >= diff) {
				dp[i][j + a[i][2]] = dp[i - 1][j] - diff;
				dp[i][j] = max(dp[i][j], dp[i - 1][j] - diff + a[i][1]);
			}
		}
	}
	for (auto& x : a) {
		if (x[0] > sum) break;
		sum += x[1];
	}
	cout << sum;
	return 0;
}
