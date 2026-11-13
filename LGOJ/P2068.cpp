#include <bits/stdc++.h>
using namespace std;
long long n, m, x, y, tree[400010];
char op;

inline int lowbit(int x) {
	return x & (-x);
}

inline void add(int x, int k) {
	for (; x <= n; x += lowbit(x)) tree[x] += k;
}

inline long long get(int x){
	long long ans = 0;
	for (; x; x -= lowbit(x)) ans += tree[x];
	return ans;
}

int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> m;
	while (m--) {
		cin >> op >> x >> y;
		if (op == 'x') {
			add(x, y);
		} else {
			cout << get(y) - get(x - 1) << endl;
		}
	}
	return 0;
}
