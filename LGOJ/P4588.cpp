#include <bits/stdc++.h>
using namespace std;
inline long long read() {
	long long ans = 0, f = 1;
	char ch = getchar();
	while (!isdigit(ch)) {
		if (ch == '-') f = -1;
		ch = getchar();
	}
	while (isdigit(ch)) {
		ans = ans * 10 + ch - '0';
		ch = getchar();
	}
	return ans * f;
}

class SegmentTree {
private:
	vector<long long> tr;
	long long n, mod;
	
	void build(int node, int l, int r) {
		if (l == r) {
			tr[node] = 1;
			return;
		}
		int mid = (l + r) >> 1;
		build(node << 1, l, mid);
		build(node << 1 | 1, mid + 1, r);
		push_up(node);
	}
	
	void push_up(int node) {
		tr[node] = (tr[node << 1] * tr[node << 1 | 1]) % mod;
	}
	
	void updateMul(int node, int l, int r, int x, long long k) {
		if (l == r) {
			tr[node] = k;
			return;
		}
		int mid = (l + r) >> 1;
		if (x <= mid) updateMul(node << 1, l, mid, x, k);
		if (x > mid) updateMul(node << 1 | 1, mid + 1, r, x, k);
		push_up(node);
	}
public:
	
	SegmentTree(int _n, int p) {
		n = _n;
		mod = p;
		tr = vector<long long>(4 * n + 5);
		build(1, 1, n);
	}
	
	void update(int x, long long k) {
		updateMul(1, 1, n, x, k);
	}
	
	int query() {
		return tr[1] % mod;
	}
};

int t, n, m, op, x;

void solve() {
	n = read(), m = read();
	SegmentTree st(n, m);
	for (int i = 1; i <= n; i++) {
		op = read(), x = read();
		if (op == 1) st.update(i, x);
		if (op == 2) st.update(x, 1);
		printf("%d\n", st.query());
	}
}

int main() {
	t = read();
	while (t--) solve();
	return 0;
}
