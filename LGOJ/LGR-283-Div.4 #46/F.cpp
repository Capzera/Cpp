#include <bits/stdc++.h>
using namespace std;
long long n, m, q, x, y, a[51];
struct node {
	int u, v, id;
	void read(int i) {
		cin >> u >> v;
		id = i;
	}
	bool operator < (const struct node &rhs) const {
		return u < rhs.u;
	}
} vec[50010], que[50010];
int ans[50010];
int main() {
	cin >> n >> m >> q;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = 1; i <= m; i++) vec[i].read(i);
	for (int i = 1; i <= q; i++) que[i].read(i);
	sort(que + 1, que + q + 1);
	for (int i = 1, j = 1; i <= q; i++) {
		while (j < que[i].u) {
			a[vec[j].u] = vec[j].v;
			j++;
		}
		ans[que[i].id] = a[que[i].v];
	}
	for (int i = 1; i <= q; i++) cout << ans[i] << "\n";
	return 0;
}
