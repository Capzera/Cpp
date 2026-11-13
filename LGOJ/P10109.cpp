#include <bits/stdc++.h>
using namespace std;
long long n, m, q, k, x, pa[301], a[301];
bool find(int u, int v) { // 当前为点u，是否能寻找到点v
	if (!u && v) return 0;
	if (pa[u] == v) return 1;
	return find(pa[u], v);
}
bool check(int u) { // 判断结点u是否可以遍历到所有的a
	for (int i = 1; i <= k; i++) {
		if (!find(a[i], u) && a[i] != u) return 0;
	}
	return 1;
}
int main() {
	cin >> n;
	for (int i = 1; i < n; i++) cin >> pa[i];
	cin >> q;
	while (q--) {
		cin >> k;
		for (int i = 1; i <= k; i++) cin >> a[i];
		for (int u = n - 1; ~u; u--) {
			if (!check(u)) continue;
			cout << u << endl;
			break;
		}
	}
	return 0;
}
