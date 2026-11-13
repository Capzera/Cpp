#include <bits/stdc++.h>
#define int long long
using namespace std;
struct node {
	long long cost, amount;
	bool operator < (const struct node& rhs) const {
		return cost < rhs.cost;
	}
};
int n, m, k, cost, amount, dist, sum, pos, cnt, ans = LLONG_MAX;
priority_queue<node> pq;
void add(int amount, int cost) {
	cnt += amount, sum += cost * amount;
}
signed main() {
	cin >> n >> m >> k;
	for (int i = 1; i <= n; i++) {
		cin >> cost >> amount >> dist;
		sum += (dist - pos) * k, pos = dist;
		add(amount, cost);
		pq.push({cost, amount});
		while (pq.size() && cnt > m) {
			node top = pq.top(); pq.pop();
			int u = top.amount, v = top.cost;
			if (cnt - u >= m) {
				add(-u, v);
			} else {
				top.amount -= cnt - m;
				add(m - cnt, v);
				pq.push(top);
			}
		}
		if (cnt >= m) ans = min(ans, sum);
	}
	if (cnt < m) return puts("-1"), 0;
	cout << ans;
	return 0;
}
