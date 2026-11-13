#include <bits/stdc++.h>
#define ll long long
using namespace std;
int read() {
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
	vector<ll> tr, lz;
	int n;
	
	void buildTree(int node, int l, int r, vector<ll>& a) {
		if (l == r) {
			tr[node] = a[l];
			return;
		}
		int mid = (l + r) >> 1;
		buildTree(node << 1, l, mid, a);
		buildTree(node << 1 | 1, mid + 1, r, a);
		push_up(node);
	}
	
	void push_down(int node, int l, int r) {
		if (lz[node]) {
			lz[node << 1] += lz[node];
			lz[node << 1 | 1] += lz[node];
			lz[node] = 0;
		}
	}
	
	void push_up(int node) {
		tr[node] = tr[node << 1] + tr[node << 1 | 1];
	}
	
	void changeAdd(int node, int l, int r, int x, int lt, int rt) {
		if (l >= lt && r <= rt) {
			lz[node] += x;
			return;
		}
		int mid = (l + r) >> 1;
		push_down(node, l, r);
		if (lt <= mid) changeAdd(node << 1, l, mid, x, lt, rt);
		if (rt > mid) changeAdd(node << 1 | 1, mid + 1, r, x, lt, rt);
		push_up(node);
	}
	
	ll querySum(int node, int l, int r, int lt, int rt) {
		if (l >= lt && r <= rt) return tr[node] + lz[node];
		int mid = (l + r) >> 1;
		ll ans = 0;
		push_down(node, l, r);
		if (lt <= mid) ans += querySum(node << 1, l, mid, lt, rt);
		if (rt > mid) ans += querySum(node << 1 | 1, mid + 1, r, lt, rt);
		return ans;
	}
	
public:
	SegmentTree(vector<ll>& a) {
		n = a.size() - 1;
		tr = lz = vector<ll>(4 * n);
		buildTree(1, 1, n, a);
	}
	
	void update(int l, int r, int x) {
		changeAdd(1, 1, n, x, l, r);
	}
	
	ll query(int l, int r) {
		return querySum(1, 1, n, l, r);
	}
};

int n, m, op, l, r, x;

int main() {
	n = read(), m = read();
	vector<ll> a(n + 1);
	for (int i = 1; i <= n; i++) a[i] = read();
	SegmentTree st(a);
	while (m--) {
		op = read(), l = read();
		if (op == 1) {
			r = read(), x = read();
			st.update(l, r, x);
		}
		if (op == 2) {
			printf("%lld\n", st.query(l, l));
		}
	}
	return 0;
}
