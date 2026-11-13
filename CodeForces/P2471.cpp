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
	void push_up(int node) {
		tr[node] = max(tr[node << 1], tr[node << 1 | 1]);
	}
	void buildTree(int node, int l, int r, const vector<int> &a) {
		if (l == r) {
			tr[node] = a[l];
			return;
		}
		int mid = (l + r) >> 1;
		buildTree(node << 1, l, mid, a);
		buildTree(node << 1 | 1, mid + 1, r, a);
		push_up(node);
	}
	int queryMax(int node, int l, int r, int lt, int rt) {
		if (lt <= l && r <= rt) return tr[node];
		int ans = 0, mid = (l + r) >> 1;
		if (lt <= mid) ans = max(ans, queryMax(node << 1, l, mid, lt, rt));
		if (rt > mid) ans = max(ans, queryMax(node << 1 | 1, mid + 1, r, lt, rt));
		return ans;
	}
public:
	SegmentTree(const vector<int>& a) {
		this->n = a.size() - 1;
		tr = vector<int> (n * 4 + 5);
		if (n > 0) buildTree(1, 1, n, a);
	}
	int query(int l, int r) {
		if (l > r) return 0;
		return queryMax(1, 1, n, l, r);
	}
	~SegmentTree(){}
};
int n, m, x, y, a[50001];
int main() {
	n = read();
	vector<int> b(n + 1);
	for (int i = 1; i <= n; i++) {
		a[i] = read(), b[i] = read();
	}
	SegmentTree st(b);
	m = read();
	while (m--) {
		x = read(), y = read();
		if (x >= y) {puts("false"); continue;}
		int l = lower_bound(a + 1, a + n + 1, x) - a;
		int r = lower_bound(a + 1, a + n + 1, y) - a;
		bool fx = a[l] == x, fy = a[r] == y;
		if (!fx) l--;
		int mx = st.query(l + 1, r - 1);
		if ((fy && mx >= b[r]) || (b[l] < b[r] && fx && fy) || (mx >= b[l] && fx)) puts("false");
		else if (r - l != a[r] - a[l] || !fx || !fy) puts("maybe");
		else puts("true");
	}
	return 0;
}
