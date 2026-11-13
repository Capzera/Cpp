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
	int n;
	vector<int> tr;
	
	void push_up(int node) {
		tr[node] = tr[node << 1] + tr[node << 1 | 1];
	}
	
	void updateAdd(int node, int l, int r, int x) {
		if (l == r) {
			tr[node]++;
			return;
		}
		int mid = (l + r) >> 1;
		if (x <= mid) updateAdd(node << 1, l, mid, x);
		if (x > mid) updateAdd(node << 1 | 1, mid + 1, r, x);
		push_up(node);
	}
	
	int querySum(int node, int l, int r, int lt, int rt) {
		if (l >= lt && r <= rt) return tr[node];
		int mid = (l + r) >> 1, ans = 0;
		if (lt <= mid) ans += querySum(node << 1, l, mid, lt, rt);
		if (rt > mid) ans += querySum(node << 1 | 1, mid + 1, r, lt, rt);
		return ans;
	}
public:
	SegmentTree(int _n) {
		n = _n;
		tr = vector<int> (4 * n + 5);
	}
	
	void update(int x) {
		updateAdd(1, 1, n, x);
	}
	
	int query(int l, int r) {
		return querySum(1, 1, n, l, r);
	}
};

int n, m, op, l, r;

int main() {
	n = read(), m = read();
	SegmentTree L(n), R(n);
	while (m--) {
		op = read(), l = read(), r = read();
		if (op == 1) {
			L.update(l), R.update(r);
		}
		if (op == 2) {
			printf("%d\n", L.query(1, r) - R.query(1, l - 1));
		}
	}
	return 0;
}
