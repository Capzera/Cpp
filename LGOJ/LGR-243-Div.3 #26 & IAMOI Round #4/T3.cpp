#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10;
long long n, m, k, h, a[N];
bool check(long long x) {
	// 假设所有木桶高度最终应该最低值为x
	priority_queue<long long> pq;
	for (int i = 1; i <= n; i++) {
		if (x - a[i] > 0) pq.push(x - a[i]);
	}
	long long c1 = m, c2 = k; // c1->1, c2->h
	while (pq.size()) {
		long long t = pq.top(); pq.pop();
		if (c2) {
			long long cnt = min(t / h, c2);
			if (cnt) pq.push(t - h * cnt);
			else cnt = 1;
			c2 -= cnt;
		} else {
			c1 -= t;
		}
		if (c1 < 0) return 0;
	}	
	return 1;
}
int main() {
	cin >> n >> m >> k >> h;
	long long l = 0, r = 0, ans = 0;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
		l = max(l, a[i]);
	}
	r = l + h * k + m + 1;
	while (l < r) {
		long long mid = l + (r - l) / 2;
		if (check(mid)) {
			ans = mid;
			l = mid + 1;
		} else r = mid;
	}
	cout << ans;
	return 0;
}
