#include <bits/stdc++.h>
#define ll long long
using namespace std;
class SegmentTree {
private:
	vector<ll> tr, lz;
	int n;
	
	void buildTree(int node, int l, int r, vector<char>& s) {
		if (l == r) {
			tr[node] = s[l] - '0';
			return;
		}
		int mid = (l + r) >> 1;
		buildTree(node << 1, l, mid, s);
		buildTree(node << 1 | 1, mid + 1, r, s);
		push_up(node);
	}
	
	void push_down(int node, int l, int r) {
		int mid = (l + r) >> 1;
		if (lz[node]) {
			tr[node << 1] = mid - l + 1 - tr[node << 1];
			tr[node << 1 | 1] = r - mid - tr[node << 1 | 1];
			lz[node << 1] ^= 1;
			lz[node << 1 | 1] ^= 1;
			lz[node] = 0;
		}
	}
	
	void push_up(int node) {
		tr[node] = tr[node << 1] + tr[node << 1 | 1];
	}
	
	void changeAdd(int node, int l, int r, int lt, int rt) {
		if (l >= lt && r <= rt) {
			tr[node] = (r - l + 1) - tr[node];
			lz[node] ^= 1;
			return;
		}
		int mid = (l + r) >> 1;
		push_down(node, l, r);
		if (lt <= mid) changeAdd(node << 1, l, mid, lt, rt);
		if (rt > mid) changeAdd(node << 1 | 1, mid + 1, r, lt, rt);
		push_up(node);
	}
	
	ll querySum(int node, int l, int r, int lt, int rt) {
		if (l >= lt && r <= rt) return tr[node];
		int mid = (l + r) >> 1;
		ll ans = 0;
		push_down(node, l, r);
		if (lt <= mid) ans += querySum(node << 1, l, mid, lt, rt);
		if (rt > mid) ans += querySum(node << 1 | 1, mid + 1, r, lt, rt);
		return ans;
	}
	
public:
	SegmentTree(vector<char> s) {
		n = s.size() - 1;
		tr = lz = vector<ll>(4 * n);
		buildTree(1, 1, n, s);
	}
	
	void update(int l, int r) {
		changeAdd(1, 1, n, l, r);
	}
	
	ll query(int l, int r) {
		return querySum(1, 1, n, l, r);
	}
};

int n, m, op, l, r;
string s;
int main() {
	scanf("%d%d", &n, &m);
	vector<char> a(n + 1);
	for (int i = 1; i <= n; i++) cin >> a[i];
	SegmentTree st(a);
	while (m--) {
		scanf("%d%d%d", &op, &l, &r);
		if (op == 0) {
			st.update(l, r);
		}
		if (op == 1) {
			printf("%lld\n", st.query(l, r));
		}
	}
	return 0;
}
