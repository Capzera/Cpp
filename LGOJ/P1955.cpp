#include <bits/stdc++.h>
using namespace std;
unordered_map<int, int> pa;
int find(int x) {
	return x == pa[x] ? x : pa[x] = find(pa[x]);
}
int merge(int x, int y, int e) {
	if (!pa.count(x)) pa[x] = x;
	if (!pa.count(y)) pa[y] = y;
	x = find(x), y = find(y);
	if (x == y) return 0;
	if (e) pa[x] = y;
	return 1;
}
struct edge {
	int x, y, e;
} a[100001];
int t, n, x, y, e;
int solve() {
	cin >> n;
	pa.clear();
	for (int i = 1; i <= n; i++) {
		cin >> a[i].x >> a[i].y >> a[i].e;
		if (a[i].e) merge(a[i].x, a[i].y, 1);
	}
	for (int i = 1; i <= n; i++) {
		if (a[i].e) continue;
		if (!merge(a[i].x, a[i].y, 0)) return 0;
	}
	return 1;
}
int main() {
	cin >> t;
	while (t--) cout << (solve() ? "YES\n" : "NO\n");
	return 0;
}

