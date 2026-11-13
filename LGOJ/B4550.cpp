#include <bits/stdc++.h>
using namespace std;
int n, m, q, op, x, y, h, w, id, curId, ans;
int a[1001][1001];
struct node {int x, y, h, w;};
vector<node> vec;
unordered_set<int> mp;
int check(int x, int y, int h, int w) {
	if (x + h - 1 > n || y + w - 1 > m) return 0;
	for (int i = 0; i < h; i++) for (int j = 0; j < w; j++)
		if (a[x + i][y + j]) return 0;
	return 1;
}
void paste(int x, int y, int h, int w, int id) {
	for (int i = 0; i < h; i++) for (int j = 0; j < w; j++) 
		a[x + i][y + j] = id;
}
void solve() {
	scanf("%d", &op);
	if (op == 1) {
		scanf("%d%d%d%d", &x, &y, &h, &w);
		if (!check(x, y, h, w)) {
			puts("Error");
			return;
		}
		cout << ++curId << '\n';
		paste(x, y, h, w, curId);
		vec.push_back({x, y, h, w});
	} else if (op == 2) {
		scanf("%d", &id);
		if (id > curId || mp.count(id)) {
			puts("Error");
			return;
		}
		x = vec[id - 1].x, y = vec[id - 1].y, h = vec[id - 1].h, w = vec[id - 1].w;
		paste(x, y, h, w, 0);
		mp.insert(id);
		puts("Remove");
	} else {
		scanf("%d%d", &x, &y);
		cout << a[x][y] << '\n';
	}
}
int main() {
	scanf("%d%d%d", &n, &m, &q);
	while (q--) solve();
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= m; j++)
			ans += a[i][j] != 0;
	cout << curId - mp.size() << " " << ans;
	return 0;
}
