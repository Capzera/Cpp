#include <bits/stdc++.h>
using namespace std;
inline int read() {
	int ans = 0, f = 1;
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
	int n;
	vector<int> tr, lz;
	
	void push_down(int node, int l, int r) {
		if (lz[node]) {
			int mid = l + (r - l >> 1);
			tr[node << 1] = mid - l + 1 - tr[node << 1];
			tr[node << 1 | 1] = r - mid - tr[node << 1 | 1];
			lz[node << 1] ^= 1;
			lz[node << 1 | 1] ^= 1;
			lz[node] = 0;
		}
	}
	
	void push_up(int node) {
		tr[node] = tr[node << 1] + tr[node << 1 | 1];
	}
	
	void updateReverse(int node, int l, int r, int lt, int rt) {
		if (l >= lt && r <= rt) {
			lz[node] ^= 1;
			tr[node] = r - l + 1 - tr[node];
			return;
		}
		int mid = l + (r - l >> 1);
		push_down(node, l, r);
		if (lt <= mid) updateReverse(node << 1, l, mid, lt, rt);
		if (rt > mid) updateReverse(node << 1 | 1, mid + 1, r, lt, rt);
		push_up(node);
	}
	
	int querySum(int node, int l, int r, int lt, int rt) {
		if (l >= lt && r <= rt) return tr[node];
		int mid = l + (r - l >> 1), ans = 0;
		push_down(node, l, r);
		if (lt <= mid) ans += querySum(node << 1, l, mid, lt, rt);
		if (rt > mid) ans += querySum(node << 1 | 1, mid + 1, r, lt, rt);
		return ans;
	}
	
public:
	SegmentTree(int _n) {
		n = _n;
		tr = lz = vector<int> (4 * n + 5);
	}
	
	void update(int l, int r) {
		updateReverse(1, 1, n, l, r);
	}
	
	int query(int l, int r) {
		return querySum(1, 1, n, l, r);
	}
};

int n, m, op, l, r;

int main() {
	n = read(), m = read();
	SegmentTree st(n);
	while (m--) {
		op = read(), l = read(), r = read();
		if (op == 0) st.update(l, r);
		if (op == 1) printf("%d\n", st.query(l, r));
	}
	return 0;
}
