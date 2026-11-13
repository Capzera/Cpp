#include <bits/stdc++.h>
using namespace std;
const int N = 1e4 + 10;
struct node {
	int t, x, y;
} a[N];
int Manhattan(node a, node b) {
	return abs(a.x - b.x) + abs(a.y - b.y);
}
int n, m, ans, dp[N];
int main() {
	cin >> m >> n;
	for (int i = 1; i <= n; i++) {
		cin >> a[i].t >> a[i].x >> a[i].y;
	}
	for (int i = 1; i <= n; i++) {
		dp[i] = 1;
		for (int j = 1; j < i; j++) {
			if (a[i].t - a[j].t >= Manhattan(a[i], a[j])) {
				dp[i] = max(dp[i], dp[j] + 1);
			}
		}
		ans = max(ans, dp[i]);
	}
	cout << ans;
	return 0;
}
