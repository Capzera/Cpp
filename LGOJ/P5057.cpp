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
	vector<long long> tr, lz;
	int n;
	
	void build(int node, int l, int r, vector<long long>& a) {
		if (l == r) {
			tr[node] = a[l];
			return;
		}
		int mid = (l + r) >> 1;
		build(node << 1, l, mid, a);
		build(node << 1 | 1, mid + 1, r, a);
		push_up(node);
	}
	
	void push_down(int node, int l, int r) {
		if (lz[node]) {
			int mid = (l + r) >> 1;
			tr[node << 1] = (mid - l + 1) - tr[node << 1];
			tr[node << 1 | 1] = (r - mid) - tr[node << 1 | 1];
			lz[node << 1] ^= 1;
			lz[node << 1 | 1] ^= 1;
			lz[node] = 0;
		}
	}
	
	void push_up(int node) {
		tr[node] = tr[node << 1] + tr[node << 1 | 1];
	}
	
	void changeRev(int node, int l, int r, int lt, int rt) {
		if (l >= lt && r <= rt) {
			tr[node] = (r - l + 1) - tr[node];
			lz[node] ^= 1;
			return;
		}
		push_down(node, l, r);
		int mid = (l + r) >> 1;
		if (lt <= mid) changeRev(node << 1, l, mid, lt, rt);
		if (rt > mid) changeRev(node << 1 | 1, mid + 1, r, lt, rt);
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
		tr = lz = vector<long long>(4 * n + 5);
		build(1, 1, n, a);
	}
	
	SegmentTree(int _n) {
		n = _n;
		tr = lz = vector<long long>(4 * n + 4);
	}
	
	void update(int l, int r) {
		changeRev(1, 1, n, l, r);
	}
	
	long long query(int l, int r) {
		return querySum(1, 1, n, l, r);
	}
};

long long n, m, op, l, r, k;

int main() {
	n = read(), m = read();
	SegmentTree st(n);
	
	while (m--) {
		op = read(), l = read();
		if (op == 1) {
			r = read();
			st.update(l, r);
		}
		if (op == 2) {
			printf("%lld\n", st.query(l, l));
		}
	}
	return 0;
}
