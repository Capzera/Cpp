#include <bits/stdc++.h>
using namespace std;

inline long long read() {
	long long ans = 0, f = 1;
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
	vector<long long> mx, tr, lz;
	
	void build(int node, int l, int r, vector<long long>& a) {
		if (l == r) {
			tr[node] = mx[node] = a[l];
			return;
		}
		int mid = (l + r) >> 1;
		build(node << 1, l, mid, a);
		build(node << 1 | 1, mid + 1, r, a);
		push_up(node);
	}
	
	void push_down(int node, int l, int r) {
		
	}
	
	void push_up(int node) {
		tr[node] = tr[node << 1] + tr[node << 1 | 1];
		mx[node] = max(mx[node << 1], mx[node << 1 | 1]);
	}
	
	void updateSqrt(int node, int l, int r, int lt, int rt) {
		if (l == r) {
			tr[node] = sqrt(tr[node]);
			mx[node] = sqrt(mx[node]);
			return;
		}
		int mid = (l + r) >> 1;
		push_down(node, l, r);
		if (lt <= mid && mx[node << 1] > 1) updateSqrt(node << 1, l, mid, lt, rt);
		if (rt > mid && mx[node << 1 | 1] > 1) updateSqrt(node << 1 | 1, mid + 1, r, lt, rt);
		push_up(node);
	}
	
	long long querySum(int node, int l, int r, int lt, int rt) {
		if (l >= lt && r <= rt) return tr[node];
		int mid = (l + r) >> 1;
		long long ans = 0;
		push_down(node, l, r);
		if (lt <= mid) ans += querySum(node << 1, l, mid, lt, rt);
		if (rt > mid) ans += querySum(node << 1 | 1, mid + 1, r, lt, rt);
		return ans;
	}
	
public:
	
	SegmentTree(vector<long long>& a) {
		n = a.size() - 1;
		tr = lz = mx = vector<long long> (4 * n + 5);
		build(1, 1, n, a);
	}
	
	void update(int l, int r) {
		updateSqrt(1, 1, n, l, r);
	}
	
	long long query(int l, int r) {
		return querySum(1, 1, n, l, r);
	}
};

long long n, m, op, l, r;

int main() {
	n = read();
	vector<long long> a(n + 1);
	for (int i = 1; i <= n; i++) a[i] = read();
	SegmentTree st(a);
	m = read();
	while (m--) {
		op = read(), l = read(), r = read();
		if (l > r) swap(l, r);
		if (op == 0) st.update(l, r);
		if (op == 1) printf("%lld\n", st.query(l, r));
	}
	return 0;
}
