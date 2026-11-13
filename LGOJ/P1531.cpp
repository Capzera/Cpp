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
	vector<int> tr;
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
		tr[node] = max(tr[node << 1], tr[node << 1 | 1]);
	}
	
	void changeAdd(int node, int l, int r, int x, int k) {
		if (l >= r) {
			tr[node] = max(tr[node], k);
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
		int ans = 0;
		if (lt <= mid) ans = max(ans, querySum(node << 1, l, mid, lt, rt));
		if (rt > mid) ans = max(ans, querySum(node << 1 | 1, mid + 1, r, lt, rt));
		return ans;
	}
	
public:
	SegmentTree(vector<int>& a) {
		n = a.size() - 1;
		tr = vector<int>(4 * n);
		buildTree(1, 1, n, a);
	}
	
	void update(int l, int r) {
		changeAdd(1, 1, n, l, r);
	}
	
	int query(int l, int r) {
		return querySum(1, 1, n, l, r);
	}
};

int n, m, l, r;
char op;

int main() {
	n = read(), m = read();
	vector<int> a(n + 1);
	for (int i = 1; i <= n; i++) a[i] = read();
	SegmentTree st(a);
	
	while (m--) {
		cin >> op, l = read(), r = read();
		if (op == 'Q') {
			printf("%d\n", st.query(l, r));
		}
		if (op == 'U') {
			st.update(l, r);
		}
	}
	return 0;
}
