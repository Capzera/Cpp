#include <bits/stdc++.h>
using namespace std;
struct node {
	int x, y;
	bool operator < (const struct node& rhs) const {
		return x < rhs.x;
	}
} a[200001];
int n, ans, dp[200001];
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> a[i].x >> a[i].y;
	}
	sort(a + 1, a + n + 1);
	for (int i = 1; i <= n; i++) {
		if (a[i].y > dp[ans]) {
			dp[++ans] = a[i].y;
			continue;
		}
		int j = lower_bound(dp + 1, dp + ans + 1, a[i].y) - dp;
		dp[j] = a[i].y;
	}
	cout << ans;
	return 0;
}
