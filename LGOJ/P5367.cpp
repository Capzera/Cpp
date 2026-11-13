#include <bits/stdc++.h>
using namespace std;
const int N = 1e6 + 10, mod = 998244353;
class SegmentTree {
private:
	vector<int> tr;
	int n;
	void push_up(int node) {
		tr[node] = tr[node << 1] + tr[node << 1 | 1];
	}
	
	void updateAdd(int node, int l, int r, int x, int k) {
		if (l == r) {
			tr[node] += k;
			return;
		}
		int mid = l + (r - l >> 1);
		if (x <= mid) updateAdd(node << 1, l, mid, x, k);
		if (x > mid) updateAdd(node << 1 | 1, mid + 1, r, x, k);
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
		tr = vector<int> (n * 4 + 5);
	}

	void update(int x) {
		updateAdd(1, 1, n, x, 1);
	}
	
	int query(int l, int r) {
		if (l > r) return 0;
		return querySum(1, 1, n, l, r);
	}
};

long long n, ans, fac = 1, a[N];

int main() {
	cin >> n;
	SegmentTree tr(n);
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
	}
	for (int i = n; i > 0; i--) {
		if (i != n) fac = fac * (n - i) % mod;
		ans = (ans + tr.query(1, a[i] - 1) * fac) % mod;
		tr.update(a[i]);
	}
	cout << ans + 1;
	return 0;
}
