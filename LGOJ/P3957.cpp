#include <bits/stdc++.h>
using namespace std;
const int N = 5e5 + 10;
int n, d, k, x[N], s[N];
long long sum, dp[N];
bool check(int mid) { // 猜测使用金币为mid时，能否使得得分 >= k
	int lo = max(d - mid, 1), hi = d + mid;
	memset(dp, -0x7f, sizeof(dp));
	deque<int> q;
	dp[0] = 0;
	for (int i = 1, j = 0; i <= n; i++) {
		while (j < i && x[i] - x[j] >= lo) {
			if (dp[j] != -0x7f7f7f7f7f7f7f7f) {
				while (q.size() && dp[q.back()] <= dp[j]) q.pop_back();
				q.push_back(j);
			}
			j++;
		}
		while (q.size() && x[i] - x[q.front()] > hi) q.pop_front();
		/*for (int j = i - 1; ~j; j--) {
			if (x[i] - x[j] < lo) continue;
			if (x[i] - x[j] > hi) break;
			dp[i] = max(dp[i], dp[j] + s[i]);
			if (dp[i] >= k) return 1;
		}*/
		if (q.size()) dp[i] = dp[q.front()] + s[i];
		if (dp[i] >= k) return 1;
	}
	return 0;
}
int main() {
	cin >> n >> d >> k;
	int l = 0, r = d, ans = -1;
	for (int i = 1; i <= n; i++) {
		cin >> x[i] >> s[i];
		if (s[i] > 0) sum += s[i];
	}
	r = max(r, x[n] + 1);
	while (l < r) {
		int mid = l + (r - l >> 1);
		if (check(mid)) {
			ans = r = mid;
		} else l = mid + 1;
	}
	cout << ans;
	return 0;
}
