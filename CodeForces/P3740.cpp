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
	vector<int> tr, lz;
	
	void push_down(int node) {
		if (lz[node]) {
			tr[node << 1] = tr[node << 1 | 1] = lz[node << 1] = lz[node << 1 | 1] = 1;
			lz[node] = 0;
		}
	}
	
	void push_up(int node) {
		tr[node] = tr[node << 1] & tr[node << 1 | 1];
	}
	
	void updateAdd(int node, int l, int r, int lt, int rt) {
		if (l >= lt && r <= rt) {
			lz[node] = 1;
			tr[node] = 1;
			return;
		}
		push_down(node);
		int mid = (l + r) >> 1;
		if (lt <= mid) updateAdd(node << 1, l, mid, lt, rt);
		if (rt > mid) updateAdd(node << 1 | 1, mid + 1, r, lt, rt);
		push_up(node);
	}
	
	int querySum(int node, int l, int r, int lt, int rt) {
		if (l >= lt && r <= rt) return tr[node];
		push_down(node);
		int mid = (l + r) >> 1, ans = 1;
		if (lt <= mid) ans &= querySum(node << 1, l, mid, lt, rt);
		if (rt > mid) ans &= querySum(node << 1 | 1, mid + 1, r, lt, rt);
		return ans;
	}
	
public:
	SegmentTree(int _n) {
		n = _n;
		tr = lz = vector<int> (4 * n + 5);
	}
	
	void update(int l, int r) {
		updateAdd(1, 1, n, l, r);
	}
	
	int query(int l, int r) {
		return querySum(1, 1, n, l, r);
	}
};

int n, m, ans, a[1005], b[1005];

int main() {
	n = read(), m = read();
	vector<int> vec;
	for (int i = 1; i <= m; i++) {
		a[i] = read(), b[i] = read();
		vec.push_back(a[i]);
		vec.push_back(b[i]);
	}
	sort(vec.begin(), vec.end());
	vec.erase(unique(vec.begin(), vec.end()), vec.end());
	int sz = vec.size();
	for (int i = 1; i < sz; i++) {
		if (vec[i] - vec[i - 1] > 1) {
			vec.push_back(vec[i - 1] + 1);
		}
	}
	sort(vec.begin(), vec.end());
	vec.erase(unique(vec.begin(), vec.end()), vec.end());
	SegmentTree st(vec.size());
	for (int i = m; i > 0; i--) {
		int l = lower_bound(vec.begin(), vec.end(), a[i]) - vec.begin() + 1;
		int r = lower_bound(vec.begin(), vec.end(), b[i]) - vec.begin() + 1;
		if (!st.query(l, r)) ans++;
		st.update(l, r);
	}
	printf("%d\n", ans);
	return 0;
}
