#include <bits/stdc++.h>
using namespace std;
long long m, s, tmp, t, dp[2];
int main() {
	cin >> m >> s >> t;
	for (int i = 1; i <= t; i++) {
		dp[1] = max(dp[0], dp[1]) + 17;
		if (m >= 10) {
			dp[0] += 60;
			m -= 10;
		} else m += 4;
		if (dp[0] >= s || dp[1] >= s) {
			cout << "Yes" << endl << i;
			return 0;
		}
	}
	cout << "No" << endl << max(dp[0], dp[1]);
	return 0;
}
/*
#include <bits/stdc++.h>
using namespace std;
long long m, s, t, tmp, dp[300010][2];
int main() {
	cin >> m >> s >> t;
	for (int i = 1; i <= t; i++) {
		dp[i][0] = dp[i - 1][0];
		dp[i][1] = max(dp[i - 1][0], dp[i - 1][1]) + 17;
		if (m >= 10) {
			dp[i][0] += 60;
			m -= 10;
		} else m += 4;
		if (max(dp[i][0], dp[i][1]) >= s) {
			cout << "Yes" << endl << i;
			return 0;
		}
	}
	cout << "No" << endl << max(dp[t][0], dp[t][1]);
	return 0;
}
*/
