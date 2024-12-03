#include <bits/stdc++.h>
using namespace std;
int n, m, op, x, y, tree[20000];

inline int lowbit(int x) {
	return x & (-x);
}

inline void add(int x, int k) {
	for(; x <= n; x += lowbit(x)) tree[x] += k;
}

inline int get(int x) {
	int ans = 0;
	for (; x; x -= lowbit(x)) ans += tree[x];
	return ans;
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> m;
	for (int i = 1; i <= n; i++) {
		cin >> x;
		add(i, x);
	}
	while (m--) {
		cin >> op >> x >> y;
		if (op == 1) {
			add(x, y);
		} else {
			cout << get(y) - get(x - 1) << endl;
		}
	}
	return 0;
}
