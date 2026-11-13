#include <bits/stdc++.h>
using namespace std;
const int N = 5e5 + 10;

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
	vector<long long> tr;
	
	void push_up(int node) {
		tr[node] = tr[node << 1] + tr[node << 1 | 1];
	}
	
	void updateAdd(int node, int l, int r, int x) {
		if (l >= r) {
			tr[node]++;
			return;
		}
		int mid = l + (r - l >> 1);
		if (x <= mid) updateAdd(node << 1, l, mid, x);
		if (x > mid) updateAdd(node << 1 | 1, mid + 1, r, x);
		push_up(node);
	}
	
	int querySum(int node, int l, int r, int lt, int rt) {
		if (l >= lt && r <= rt) return tr[node];
		int mid = l + (r - l >> 1), ans = 0;
		if (lt <= mid) ans += querySum(node << 1, l, mid, lt, rt);
		if (rt > mid) ans += querySum(node << 1 | 1, mid + 1, r, lt, rt);
		return ans;
	}
public:
	SegmentTree(int _n) {
		n = _n;
		tr = vector<long long>(4 * n + 5);
	}
	
	void update(int x) {
		updateAdd(1, 1, n, x);
	}
	
	int query(int l, int r) {
		return querySum(1, 1, n, l, r);
	}
};


long long n, ans, a[N], b[N];

int main() {
	n = read();
	for (int i = 1; i <= n; i++) {
		a[i] = read();
		b[i] = a[i];
	}
	sort(b + 1, b + n + 1);
	int m = unique(b + 1, b + n + 1) - (b + 1);
	for (int i = 1; i <= n; i++) {
		a[i] = lower_bound(b + 1, b + m + 1, a[i]) - b;
	}
	SegmentTree st(n);
	for (int i = 1; i <= n; i++) {
		ans += st.query(a[i] + 1, n);
		st.update(a[i]);
	}
	cout << ans;
	return 0;
}
