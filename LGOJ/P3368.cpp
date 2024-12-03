#include <bits/stdc++.h>
using namespace std;
int n, m, k, x, y, op, a[500010], tree[2000010];

inline int lowbit(int x) {
	return x & (-x);
}

inline void add(int x, int k) {
	for (; x <= n; x += lowbit(x)) tree[x] += k;
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
		cin >> a[i];
	}
	for (int i = n; i > 0; i--) {
		a[i] -= a[i - 1];
		add(i, a[i]);
	}
	while (m--) {
		cin >> op >> x;
		if (op == 1) {
			cin >> y >> k;
			add(x, k);
			add(y + 1, -k);
		} else {
			cout << get(x) << endl;
		}
	}
	return 0;
}
