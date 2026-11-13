#include <bits/stdc++.h>
#define ll long long
#define pii pair<int, int>
using namespace std;
const int N = 1e5 + 10;
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
	vector<ll> tr, lz, add;
	int n;
	
	void push_up(int node) {
		tr[node] = max(tr[node << 1], tr[node << 1 | 1]);
	}
	
	void push_down(int node, int l, int r) {
		if (lz[node] != LLONG_MIN) {
			tr[node << 1] = lz[node << 1] = lz[node];
			tr[node << 1 | 1] = lz[node << 1 | 1] = lz[node];
			lz[node] = LLONG_MIN;
			add[node << 1] = add[node << 1 | 1] = 0;
		} else if (add[node]) {
			tr[node << 1] += add[node];
			tr[node << 1 | 1] += add[node];
			if (lz[node << 1] != LLONG_MIN) lz[node << 1] += add[node];
			else add[node << 1] += add[node];
			if (lz[node << 1 | 1] != LLONG_MIN) lz[node << 1 | 1] += add[node];
			else add[node << 1 | 1] += add[node];
			add[node] = 0;
		}
	}
	
	void buildTree(int node, int l, int r, vector<ll>& a) {
		lz[node] = LLONG_MIN;
		if (l == r) {
			tr[node] = a[l];
			return;
		}
		int mid = (l + r) >> 1;
		buildTree(node << 1, l, mid, a);
		buildTree(node << 1 | 1, mid + 1, r, a);
		push_up(node);
	}
	
	void changeAssign(int node, int l, int r, int x, int lt, int rt) {
		if (l >= lt && r <= rt) {
			tr[node] = lz[node] = x;
			add[node] = 0;
			return;
		}
		push_down(node, l, r);
		int mid = (l + r) >> 1;
		if (lt <= mid) changeAssign(node << 1, l, mid, x, lt, rt);
		if (rt > mid) changeAssign(node << 1 | 1, mid + 1, r, x, lt, rt);
		push_up(node);
	}
	
	void changeAdd(int node, int l, int r, int x, int lt, int rt) {
		if (l >= lt && r <= rt) {
			tr[node] += x;
			if (lz[node] != LLONG_MIN) lz[node] += x;
			else add[node] += x;
			return;
		}
		push_down(node, l, r);
		int mid = (l + r) >> 1;
		if (lt <= mid) changeAdd(node << 1, l, mid, x, lt, rt);
		if (rt > mid) changeAdd(node << 1 | 1, mid + 1, r, x, lt, rt);
		push_up(node);
	}
	
	long long queryMaximum(int node, int l, int r, int lt, int rt) {
		if (l >= lt && r <= rt) return tr[node];
		push_down(node, l, r);
		long long ans = LLONG_MIN;
		int mid = (l + r) >> 1;
		if (lt <= mid) ans = max(ans, queryMaximum(node << 1, l, mid, lt, rt));
		if (rt > mid) ans = max(ans, queryMaximum(node << 1 | 1, mid + 1, r, lt, rt));
		return ans;
	}
	
public:
	SegmentTree(vector<ll>& a) {
		n = a.size() - 1;
		tr = lz = add = vector<ll>(4 * n);
		buildTree(1, 1, n, a);
	}
	
	void updateAssign(int l, int r, int x) {
		changeAssign(1, 1, n, x, l, r);
	}
	
	void updateAdd(int l, int r, int x) {
		changeAdd(1, 1, n, x, l, r);
	}
	
	long long query(int l, int r) {
		return queryMaximum(1, 1, n, l, r);
	}
	
	~SegmentTree() {}
};
ll n, q, op, l, r, x;
int main() {
	n = read(), q = read();
	vector<ll> a(n + 1);
	for (int i = 1; i <= n; i++) a[i] = read();
	SegmentTree st(a);
	while (q--) {
		op = read(), l = read(), r = read();
		if (op == 1) {
			x = read();
			st.updateAssign(l, r, x);
		}
		if (op == 2) {
			x = read();
			st.updateAdd(l, r, x);
		}
		if (op == 3) {
			printf("%lld\n", st.query(l, r));
		}
	}
	return 0;
}
