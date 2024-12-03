#include <bits/stdc++.h>
#define int long long
using namespace std;

signed main() {
	int n, ans = 0;
	cin >> n;
	vector<vector<int> > a(n, vector<int>(3));
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < 3; j++) cin >> a[i][j];
	}
	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			int flag = 1, s1 = 0, s2 = 0;
			for (int k = 0; k < 3; k++) {
				if (abs(a[i][k] - a[j][k]) > 5) flag = 0;
				s1 += a[i][k], s2 += a[j][k];
			}
			if (abs(s1 - s2) > 10) flag = 0;
			ans += flag;
		}
	}
	cout << ans;
	return 0;
}
