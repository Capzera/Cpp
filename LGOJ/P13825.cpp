#include <bits/stdc++.h>
#define ull unsigned long long
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
	vector<ull> tr, lz, lc, rc;
	
	int build(int l, int r) {
		tr.push_back(1LL * (l + r) * (r - l + 1) / 2);
		lz.push_back(0);
		lc.push_back(0);
		rc.push_back(0);
		return tr.size() - 1;
	}
	
	void push_down(int node, int l, int r) {
		if (!lz[node]) return;
		int mid = l + (r - l >> 1);
		if (!lc[node]) lc[node] = build(l, mid);
		if (!rc[node]) rc[node] = build(mid + 1, r);
		tr[lc[node]] += lz[node] * (mid - l + 1);
		tr[rc[node]] += lz[node] * (r - mid);
		lz[lc[node]] += lz[node];
		lz[rc[node]] += lz[node];
		lz[node] = 0;
	}
	
	void push_up(int node, int l, int r) {
		int mid = l + (r - l >> 1);
		ull lt = lc[node] ? tr[lc[node]] : 1ll * (l + mid) * (mid - l + 1) / 2;
		ull rt = rc[node] ? tr[rc[node]] : 1ll * (mid + 1 + r) * (r - mid) / 2;
		tr[node] = lt + rt;
	}
	
	void changeAdd(int node, int l, int r, int lt, int rt, ull k) {
		if (l >= lt && r <= rt) {
			tr[node] += k * (r - l + 1) ;
			lz[node] += k;
			return;
		}
		push_down(node, l, r);
		int mid = l + (r - l >> 1);
		if (lt <= mid) {
			if (!lc[node]) lc[node] = build(l, mid);
			changeAdd(lc[node], l, mid, lt, rt, k);
		}
		if (rt > mid) {
			if (!rc[node]) rc[node] = build(mid + 1, r);
			changeAdd(rc[node], mid + 1, r, lt, rt, k);
		}
		push_up(node, l, r);
	}
	
	ull querySum(int node, int l, int r, int lt, int rt) {
		if (!node) {
			l = max(l, lt);
			r = min(r, rt);
			if (l > r) return 0;
			return 1ll * (l + r) * (r - l + 1) / 2;
		}
		if (l >= lt && r <= rt) return tr[node];
		push_down(node, l, r);
		ull ans = 0;
		int mid = l + (r - l >> 1);
		if (lt <= mid) ans += querySum(lc[node], l, mid, lt, rt);
		if (rt > mid) ans += querySum(rc[node], mid + 1, r, lt, rt);
		return ans;
	}
	
public:
	SegmentTree(int _n) {
		n = _n;
		tr.push_back(0);
		lz.push_back(0);
		lc.push_back(0);
		rc.push_back(0);
		build(1, n);
	}
	
	void updateAdd(int l, int r, ull x) {
		changeAdd(1, 1, n, l, r, x);
	}
	
	ull query(int l, int r) {
		return querySum(1, 1, n, l, r);
	}

	~SegmentTree() {}
};

ull n, q, p, x, y, k, op;

int main() {
	n = read(), q = read();
	SegmentTree st(n);
	while (q--) {
		op = read(), x = read(), y = read();
		if (op == 1) {
			k = read();
			st.updateAdd(x, y, k);
		}
		if (op == 2) {
			cout << st.query(x, y) << '\n';
		}
	}
}
//2 5 6 5 6
