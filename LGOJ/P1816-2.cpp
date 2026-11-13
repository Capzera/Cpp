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
	vector<int> tr;
	
	void build(int node, int l, int r, vector<int>& a) {
		if (l >= r) {
			tr[node] = a[l];
			return;
		}
		int mid = l + (r - l >> 1);
		build(node << 1, l, mid, a);
		build(node << 1 | 1, mid + 1, r, a);
		push_up(node);
	}
	
	void push_up(int node) {
		tr[node] = min(tr[node << 1], tr[node << 1 | 1]);
	}
	
	int queryMinimum(int node, int l, int r, int lt, int rt) {
		if (l >= lt && r <= rt) return tr[node];
		int mid = l + (r - l >> 1), ans = INT_MAX;
		if (lt <= mid) ans = min(ans, queryMinimum(node << 1, l, mid, lt, rt));
		if (rt > mid) ans = min(ans, queryMinimum(node << 1 | 1, mid + 1, r, lt, rt));
		return ans;
	}
public:
	SegmentTree(vector<int>& a) {
		n = a.size() - 1;
		tr = vector<int>(4 * n + 5);
		build(1, 1, n, a);
	}
	
	int query(int l, int r) {
		return queryMinimum(1, 1, n, l, r);
	}
};

int n, m, l, r;

int main() {
	n = read(), m = read();
	vector<int> a(n + 1);
	for (int i = 1; i <= n; i++) a[i] = read();
	SegmentTree st(a);
	while (m--) {
		l = read(), r = read();
		printf("%d ", st.query(l, r));
	}
	return 0;
}
