#include <bits/stdc++.h>
using namespace std;
struct node {
	int w, s, t;
	bool operator < (const struct node& rhs) const {
		return t < rhs.t;
	}
} a[200010];
int n, ans;
priority_queue<int> pq;
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i].w >> a[i].s >> a[i].t;
	sort(a + 1, a + n + 1);
	for (long long i = 1, sum = 0; i <= n; i++) {
		sum += a[i].s - a[i].w;
		pq.push(a[i].w - a[i].s);
		while (pq.size() && sum < 0) {
			sum += pq.top(); pq.pop();
			ans++;
		}
	}
	cout << ans;
	return 0;
}
