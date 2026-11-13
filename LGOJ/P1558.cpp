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

class SegmentTree{
private:
	int n;
	vector<int> tr, lz;
	
	void build(int node, int l, int r) {
		if (l == r) {
			tr[node] = 2;
			return;
		}
		int mid = (l + r) >> 1;
		build(node << 1, l, mid);
		build(node << 1 | 1, mid + 1, r);
		push_up(node);
	}
	
	void push_down(int node, int l, int r) {
		if (lz[node]) {
			tr[node << 1] = tr[node << 1 | 1] = (1 << lz[node]);
			lz[node << 1] = lz[node << 1 | 1] = lz[node];
			lz[node] = 0;
		}
	}
	
	void push_up(int node) {
		tr[node] = tr[node << 1] | tr[node << 1 | 1];
	}
	
	void updateAss(int node, int l, int r, int lt, int rt, int c) {
		if (l >= lt && r <= rt) {
			lz[node] = c;
			tr[node] = (1 << c);
			return;
		}
		push_down(node, l, r);
		int mid = (l + r) >> 1;
		if (lt <= mid) updateAss(node << 1, l, mid, lt, rt, c);
		if (rt > mid) updateAss(node << 1 | 1, mid + 1, r, lt, rt, c);
		push_up(node);
	}
	
	int queryCnt(int node, int l, int r, int lt, int rt) {
		if (l >= lt && r <= rt) return tr[node];
		int ans = 0, mid = (l + r) >> 1;
		push_down(node, l, r);
		if (lt <= mid) ans |= queryCnt(node << 1, l, mid, lt, rt);
		if (rt > mid) ans |= queryCnt(node << 1 | 1, mid + 1, r, lt, rt);
		return ans;
	}
public:
	SegmentTree(int _n) {
		n = _n;
		tr = lz = vector<int> (4 * n + 5);
		build(1, 1, n);
	}
	
	void update(int a, int b, int c) {
		updateAss(1, 1, n, a, b, c);
	}
	
	int query(int l, int r) {
		return queryCnt(1, 1, n, l, r);
	}
};

int n, k, m, a, b, c;
char op;

int main() {
	n = read(), k = read(), m = read();
	SegmentTree st(n);
	while (m--) {
		cin >> op, a = read(), b = read();
		if (a > b) swap(a, b);
		if (op == 'C') {
			c = read();
			st.update(a, b, c);
		}
		if (op == 'P') {
			printf("%d\n", __builtin_popcount(st.query(a, b)));
		}
	}
	return 0;
}
https://times1771771760.subxiandan.top:9604/v2b/paopaogou/api/v1/client/subscribe?token=241f5c3dbdd19a4183e6e7ff2aa0d3e3
