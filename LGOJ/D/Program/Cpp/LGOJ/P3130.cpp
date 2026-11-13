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
	vector<ll> mn, tr, add;
	int n;
	
	void push_up(int node) {
		mn[node] = min(mn[node << 1], mn[node << 1 | 1]);
		tr[node] = tr[node << 1] + tr[node << 1 | 1];
	}
	
	void push_down(int node, int l, int r) {
		int mid = (l + r) >> 1;
		if (add[node]) {
			tr[node << 1] += add[node] * (mid - l + 1);
			tr[node << 1 | 1] += add[node] * (r - mid);
			mn[node << 1] += add[node];
			mn[node << 1 | 1] += add[node];
			add[node << 1] += add[node];
			add[node << 1 | 1] += add[node];
			add[node] = 0;
		}
	}
	
	void buildTree(int node, int l, int r, vector<ll>& a) {
		if (l == r) {
			tr[node] = mn[node] = a[l];
			return;
		}
		int mid = (l + r) >> 1;
		buildTree(node << 1, l, mid, a);
		buildTree(node << 1 | 1, mid + 1, r, a);
		push_up(node);
	}
	
	
	void changeAdd(int node, int l, int r, ll x, int lt, int rt) {
		if (l >= lt && r <= rt) {
			tr[node] += x * (r - l + 1);
			add[node] += x;
			mn[node] += x;
			return;
		}
		push_down(node, l, r);
		int mid = (l + r) >> 1;
		if (lt <= mid) changeAdd(node << 1, l, mid, x, lt, rt);
		if (rt > mid) changeAdd(node << 1 | 1, mid + 1, r, x, lt, rt);
		push_up(node);
	}
	
	long long queryMinimum(int node, int l, int r, int lt, int rt) {
		if (l >= lt && r <= rt) return mn[node];
		push_down(node, l, r);
		long long ans = LLONG_MAX;
		int mid = (l + r) >> 1;
		if (lt <= mid) ans = min(ans, queryMinimum(node << 1, l, mid, lt, rt));
		if (rt > mid) ans = min(ans, queryMinimum(node << 1 | 1, mid + 1, r, lt, rt));
		return ans;
	}
	
	long long querySum(int node, int l, int r, int lt, int rt) {
		if (l >= lt && r <= rt) return tr[node];
		push_down(node, l, r);
		long long ans = 0;
		int mid = (l + r) >> 1;
		if (lt <= mid) ans += querySum(node << 1, l, mid, lt, rt);
		if (rt > mid) ans += querySum(node << 1 | 1, mid + 1, r, lt, rt);
		return ans;
	}
	
public:
	SegmentTree(vector<ll>& a) {
		n = a.size() - 1;
		tr = mn = add = vector<ll>(n << 2);
		buildTree(1, 1, n, a);
	}
	
	void updateAdd(int l, int r, ll x) {
		changeAdd(1, 1, n, x, l, r);
	}
	
	long long Sum(int l, int r) {
		return querySum(1, 1, n, l, r);
	}
	
	long long Min(int l, int r) {
		return queryMinimum(1, 1, n, l, r);
	}
	
	~SegmentTree() {}
};
ll n, q, l, r, x;
char op;
int main() {
	cin >> n >> q;
	vector<ll> a(n + 1);
	for (int i = 1; i <= n; i++) cin >> a[i];
	SegmentTree st(a);
	while (q--) {
		cin >> op >> l >> r;
		if (op == 'M') {
			printf("%lld\n", st.Min(l, r));
		}
		if (op == 'P') {
			x = read();
			st.updateAdd(l, r, x);
		}
		if (op == 'S') {
			printf("%lld\n", st.Sum(l, r));
		}
	}
	return 0;
}
1 2 3 4 5 6 7 8 9 0
1 1 1 1 1 1 1
    1 1 1 1 
          1 1 1 1 1
