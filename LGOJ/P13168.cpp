#include <bits/stdc++.h>
using namespace std;
long long n, k;
struct pancake {
	long long r, h, s;
	void read() {
		cin >> r >> h;
		s = 2 * r * h;
	}
	bool operator < (const struct pancake& rhs) const {
		return r > rhs.r;
	}
} a[1001];
void solve() {
	cin >> n >> k;
	for (int i = 1; i <= n; i++) a[i].read();
	sort(a + 1, a + n + 1);
	long long ans = 0;
	for (int i = 1; i <= n; i++) {
		long long sum = a[i].r * a[i].r + a[i].s;
		priority_queue<long long, vector<long long>, greater<long long> > pq;
		for (int j = i + 1; j <= n; j++) {
			pq.push(a[j].s);
			sum += a[j].s;
			if (pq.size() > k - 1) {
				sum -= pq.top();
				pq.pop();
			}
		}
		ans = max(ans, sum);
	}
	printf("%.9lf", 3.1415926 * ans);
}
int main() {
	int t = 1;
	cin >> t;
	for (int i = 1; i <= t; i++) {
		printf("Case #%d: ", i);
		solve();
		puts("");
	}
	return 0;
}
