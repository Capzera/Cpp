#include <bits/stdc++.h>
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
	vector<int> mn, lz;
	void build(int node, int l, int r, vector<int>& a) {
		if (l >= r) {
			mn[node] = a[l];
			return;
		}
		int mid = l + (r - l >> 1);
		build(node << 1, l, mid, a);
		build(node << 1 | 1, mid + 1, r, a);
		push_up(node);
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
	
	void push_up(int node) {
		mn[node] = min(mn[node << 1], mn[node << 1 | 1]);
	}
	
	void updateAdd(int node, int l, int r, int lt, int rt, long long k) {
		if (l >= lt && r <= rt) {
			mn[node] += k;
			lz[node] += k;
			return;
		}
		push_down(node);
		int mid = l + (r - l >> 1);
		if (lt <= mid) updateAdd(node << 1, l, mid, lt, rt, k);
		if (rt > mid) updateAdd(node << 1 | 1, mid + 1, r, lt, rt, k);
		push_up(node);
	}
	
	int minQuery(int node, int l, int r, int lt, int rt) {
		if (l >= lt && r <= rt) return mn[node];
		push_down(node);
		int ans = INT_MAX, mid = l + (r - l >> 1);
		if (lt <= mid) ans = min(ans, minQuery(node << 1, l, mid, lt, rt));
		if (rt > mid) ans = min(ans, minQuery(node << 1 | 1, mid + 1, r, lt, rt));
		return ans;
	}
public:
	SegmentTree() {}
	SegmentTree(vector<int> &a) {
		n = a.size();
		mn = lz = vector<int> (n * 4);
		build(1, 1, n, a);
	}
	
	void update(int l, int r, long long k) {
		updateAdd(1, 1, n, l, r, k);
	}
	
	int query(int l, int r) {
		return minQuery(1, 1, n, l, r);
	}
	~SegmentTree() {}
};

int n, k;
long long ans;

int main() {
	n = read(), k = read();
	vector<int> a(n + 1);
	for (int i = 1; i <= n; i++) {
		a[i] = read();
		ans += a[i];
	}
	SegmentTree tr(a);
	for (int l = 1; l <= n - k + 1; l++) {
		int r = l + k - 1, mn = tr.query(l, r);
		if (mn) {
			ans -= 1ll * mn * (k - 1);
			tr.update(l, r, -mn);
		}
	}
	printf("%lld", ans);
	return 0;
}
