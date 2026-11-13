#include <bits/stdc++.h>
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
	vector<int> tr, lz;
	int n;
	
	void buildTree(int node, int l, int r, vector<int>& a) {
		if (l == r) {
			tr[node] = a[l];
			return;
		}
		int mid = (l + r) >> 1;
		buildTree(node << 1, l, mid, a);
		buildTree(node << 1 | 1, mid + 1, r, a);
		push_up(node);
	}
	
	void push_up(int node) {
		tr[node] = tr[node << 1] + tr[node << 1 | 1];
	}
	
	void changeAdd(int node, int l, int r, int x, long long k) {
		if (l >= r) {
			tr[node] += k;
			return;
		}
		int mid = (l + r) >> 1;
		if (x <= mid) changeAdd(node << 1, l, mid, x, k);
		if (x > mid) changeAdd(node << 1 | 1, mid + 1, r, x, k);
		push_up(node);
	}
	
	int querySum(int node, int l, int r, int lt, int rt) {
		if (l >= lt && r <= rt) return tr[node];
		int mid = (l + r) >> 1;
		long long ans = 0;
		if (lt <= mid) ans += querySum(node << 1, l, mid, lt, rt);
		if (rt > mid) ans += querySum(node << 1 | 1, mid + 1, r, lt, rt);
		return ans;
	}
	
public:
	SegmentTree(vector<int>& a) {
		n = a.size() - 1;
		tr = vector<int>(4 * n);
		buildTree(1, 1, n, a);
	}
	
	void update(int x, long long k) {
		changeAdd(1, 1, n, x, k);
	}
	
	long long query(int l, int r) {
		return querySum(1, 1, n, l, r);
	}
};

int n, m, op, x, k;

int main() {
	n = read(), m = read();
	vector<int> a(n + 1);
	for (int i = 1; i <= n; i++) a[i] = read();
	SegmentTree st(a);
	while (m--) {
		op = read(), x = read(), k = read();
		if (op == 1) {
			st.update(x, k);
		}
		if (op == 2) {
			printf("%lld\n", st.query(x, k));
		}
	}
	return 0;
}
