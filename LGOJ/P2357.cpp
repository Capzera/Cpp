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

int n, m, op, l, r, x;

class SegmentTree {
private:
	int n;
	vector<long long> tr, lz;
	
	void build(int node, int l, int r, vector<long long>& a) {
		if (l >= r) {
			tr[node] = a[l];
			return;
		}
		int mid = l + (r - l >> 1);
		build(node << 1, l, mid, a);
		build(node << 1 | 1, mid + 1, r, a);
		push_up(node);
	}
	
	void push_down(int node, int l, int r) {
		if (lz[node]) {
			int mid = l + (r - l >> 1);
			tr[node << 1] += lz[node] * (mid - l + 1);
			tr[node << 1 | 1] += lz[node] * (r - mid);
			lz[node << 1] += lz[node];
			lz[node << 1 | 1] += lz[node];
			lz[node] = 0;
		}
	}
	
	void push_up(int node) {
		tr[node] = tr[node << 1] + tr[node << 1 | 1];
	}
	
	void updateAdd(int node, int l, int r, int lt, int rt, long long x) {
		if (l >= lt && r <= rt) {
			lz[node] += x;
			tr[node] += x * (r - l + 1);
			return;
		}
		int mid = l + (r - l >> 1);
		push_down(node, l, r);
		if (lt <= mid) updateAdd(node << 1, l, mid, lt, rt, x);
		if (rt > mid) updateAdd(node << 1 | 1, mid + 1, r, lt, rt, x);
		push_up(node);
	}
	
	long long querySum(int node, int l, int r, int lt, int rt) {
		if (l >= lt && r <= rt) return tr[node];
		int mid = l + (r - l >> 1);
		long long ans = 0;
		push_down(node, l, r);
		if (lt <= mid) ans += querySum(node << 1, l, mid, lt, rt);
		if (rt > mid) ans += querySum(node << 1 | 1, mid + 1, r, lt, rt);
		return ans;
	}
public:
	SegmentTree(vector<long long>& a) {
		n = a.size() - 1;
		tr = lz = vector<long long>(4 * n + 5);
		build(1, 1, n, a);
	}
	
	void update(int l, int r, long long x) {
		updateAdd(1, 1, n, l, r, x);
	}
	
	long long query(int l, int r) {
		return querySum(1, 1, n, l, r);
	}
};

int main() {
	n = read(), m = read();
	vector<long long> a(n + 1);
	for (int i = 1; i <= n; i++) a[i] = read();
	SegmentTree st(a);
	while (m--) {
		op = read();
		if (op == 1) {
			l = read(), r = read(), x = read();
			st.update(l, r, x);
		}
		if (op == 2) {
			x = read();
			st.update(1, 1, x);
		}
		if (op == 3) {
			x = read();
			st.update(1, 1, -x);
		}
		if (op == 4) {
			l = read(), r = read();
			printf("%lld\n", st.query(l, r));
		}
		if (op == 5) {
			printf("%lld\n", st.query(1, 1));
		}
	}
	return 0;
}
