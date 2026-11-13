#include <bits/stdc++.h>
#define ll long long
#define INF -1e18
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
	vector<ll> mx, tr;
	int n;
	
	void push_up(int node) {
		tr[node] = tr[node << 1] + tr[node << 1 | 1];
		mx[node] = max(mx[node << 1], mx[node << 1 | 1]);
	}
	
	void buildTree(int node, int l, int r, vector<ll>& a) {
		if (l == r) {
			tr[node] = mx[node] = a[l];
			return;
		}
		int mid = (l + r) >> 1;
		buildTree(node << 1, l, mid, a);
		buildTree(node << 1 | 1, mid + 1, r, a);
		push_up(node);
	}
	
	
	void change(int node, int l, int r, ll x, int lt, int rt) {
		if (l == r) {
			tr[node] = mx[node] = x;
			return;
		}
		int mid = (l + r) >> 1;
		if (lt <= mid) change(node << 1, l, mid, x, lt, rt);
		if (rt > mid) change(node << 1 | 1, mid + 1, r, x, lt, rt);
		push_up(node);
	}
	
	long long queryMaximum(int node, int l, int r, int lt, int rt) {
		if (l >= lt && r <= rt) return mx[node];
		long long ans = 0;
		int mid = (l + r) >> 1;
		if (lt <= mid) ans = max(ans, queryMaximum(node << 1, l, mid, lt, rt));
		if (rt > mid) ans = max(ans, queryMaximum(node << 1 | 1, mid + 1, r, lt, rt));
		return ans;
	}
	
	long long querySum(int node, int l, int r, int lt, int rt) {
		if (l >= lt && r <= rt) return tr[node];
		long long ans = 0;
		int mid = (l + r) >> 1;
		if (lt <= mid) ans += querySum(node << 1, l, mid, lt, rt);
		if (rt > mid) ans += querySum(node << 1 | 1, mid + 1, r, lt, rt);
		return ans;
	}
	
public:
	SegmentTree(vector<ll>& a) {
		n = a.size() - 1;
		tr = mx = vector<ll>(n << 2);
		buildTree(1, 1, n, a);
	}
	
	void update(int x, ll k) {
		change(1, 1, n, k, x, x);
	}
	
	long long Sum(int l, int r) {
		return querySum(1, 1, n, l, r) * queryMaximum(1, 1, n, l, r);
	}
	
	~SegmentTree() {}
};
long long op, n, q, l, r, x;
int main() {
	n = read(), q = read();
	vector<ll> a(n + 1);
	for (int i = 1; i <= n; i++) a[i] = read();
	SegmentTree st(a);
	while (q--) {
		op = read(), l = read(), r = read();
		if (op == 1) {
			printf("%lld\n", st.Sum(l, r));
		}
		if (op == 2) {
			st.update(l, r);
		}
	}
	return 0;
}
