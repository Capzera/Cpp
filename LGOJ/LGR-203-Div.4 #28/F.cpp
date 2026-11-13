#include <bits/stdc++.h>
#define int long long
using namespace std;
int n, m, op, l, r, c1, c2;
int tree[5001], grass[5001];
signed main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> m;
	while (m--) {
		cin >> op >> l >> r;
		if (op == 1) {
			for (int i = l; i < r; i++) {
				grass[i] = 1;
				if (i > l) tree[i] = 1;
			}
		}
		if (op == 2) {
			for (int i = l; i <= r; i++) {
				tree[i] = 1;
				if (i < r) grass[i] = 1;
			}
		}
	}
	for (int i = 1; i <= n; i++) c1 += tree[i] ^ 1;
	for (int i = 1; i < n; i++) c2 += grass[i] ^ 1;
	cout << c1 << " " << c2;
	return 0;
}
/*
tree[i] 表示在i点上树的情况
(1..m)
grass[i] 表示i .. i + 1 之间的草地
(1..m-1)
*/
