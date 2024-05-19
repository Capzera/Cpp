#include <bits/stdc++.h>
using namespace std;
int nums[201], cnt[201];
int main() {
	int n, a, b;
	cin >> n >> a >> b;
	for (int i = 1; i <= n; i++) cin >> nums[i];
	queue<int> q;
	q.push(a); cnt[a] = 0;
	while (q.size()) {
		auto u = q.front(); q.pop();
		int v1 = u - nums[u], v2 = u + nums[u];
		if (v1 > 0 && v1 <= n) {
			q.push(v1);
			cnt[v1] = cnt[u] + 1;
		}
		if (v2 > 0 && v2 <= n) {
			q.push(v2);
			cnt[v2] = cnt[u] + 1;
		}
	}
	cout << cnt[b];
	return 0;
}
