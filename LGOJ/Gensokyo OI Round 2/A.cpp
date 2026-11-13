#include <bits/stdc++.h>
using namespace std;
const int N = 1e6 + 10;
int n, k, x, mx, a[N];
vector<int> mp(1000001);
long long ans;
struct SegmentTree {
private:
	int n;
	vector<int> lz, mn;
	void build(int node, int l, int r, vector<int> &a) {
		if (l >= r) {
			mn[node] = a[l];
			return;
		}
		int mid = l + r >> 1;
		build(node << 1, l, mid, a);
		build(node << 1 | 1, mid + 1, r, a);
		push_up(node);
	}
	void push_up(int node) {
		mn[node] = min(mn[node << 1], mn[node << 1 | 1]);
	}
	void push_down(int node) {
		if (lz[node]) {
			mn[node << 1] += lz[node];
			mn[node << 1 | 1] += lz[node];
			lz[node << 1] += lz[node];
			lz[node << 1 | 1] += lz[node];
			lz[node] = 0;
		}
	}
	void update(int node, int l, int r, int lt, int rt, int k) {
		if (l >= lt && r <= rt) {
			mn[node] += k;
			lz[node] += k;
			return;
		}
		push_down(node);
		int mid = l + r >> 1;
		if (mid >= lt) update(node << 1, l, mid, lt, rt, k);
		if (mid < rt) update(node << 1 | 1, mid + 1, r, lt, rt, k);
		push_up(node);
	}
	int query(int node, int l, int r, int lt, int rt) {
		if (l >= lt && r <= rt) return mn[node];
		push_down(node);
		int mid = l + r >> 1, ans = INT_MAX;
		if (mid >= lt) ans = min(ans, query(node << 1, l, mid, lt, rt));
		if (mid < rt) ans = min(ans, query(node << 1 | 1, mid + 1, r, lt, rt));
		return ans;
	}
public:
	SegmentTree(int _n, vector<int>& a) {
		n = _n;
		lz = mn = vector<int> (4 * n + 5);
		build(1, 1, n, a);
	}
	void Add(int l, int r, int k) {
		update(1, 1, n, l, r, k);
	}
	int Min(int l, int r) {
		return query(1, 1, n, l, r);
	}
};
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> k;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
		mp[a[i]]++;
		mx = max(mx, a[i]);
	}
	SegmentTree st(mx, mp);
	for (int x = mx - k; x >= k + 1; x--) {
		int l = x - k, r = x + k, cnt = st.Min(l, r);
		if (cnt) {
			ans += 1ll * cnt * x;
			st.Add(l, r, -cnt);
		}
	}
	cout << ans;
	return 0;
}
