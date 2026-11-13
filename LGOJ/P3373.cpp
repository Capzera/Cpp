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
	int n, p;
	vector<long long> tr, lz1, lz2;
	
	void push_down(int node, int l, int r) {
		int mid = (l + r) >> 1;
		if (lz2[node] != 1) {
			tr[node << 1] = (tr[node << 1] * lz2[node]) % p;
			tr[node << 1 | 1] = (tr[node << 1 | 1] * lz2[node]) % p;
			lz1[node << 1] = (lz1[node << 1] * lz2[node]) % p;
			lz1[node << 1 | 1] = (lz1[node << 1 | 1]* lz2[node]) % p;
			lz2[node << 1] = (lz2[node << 1]* lz2[node]) % p;
			lz2[node << 1 | 1] = (lz2[node << 1 | 1]* lz2[node]) % p;
			lz2[node] = 1;
		}
		if (lz1[node]) {
			tr[node << 1] = (tr[node << 1] + lz1[node] * (mid - l + 1)) % p;
			tr[node << 1 | 1] = (tr[node << 1 | 1] + lz1[node] * (r - mid)) % p;
			lz1[node << 1] = (lz1[node << 1] + lz1[node]) % p;
			lz1[node << 1 | 1] = (lz1[node << 1 | 1] + lz1[node]) % p;
			lz1[node] = 0;
		}
	}
	
	void push_up(int node) {
		tr[node] = (tr[node << 1] + tr[node << 1 | 1]) % p;
	}
	
	void buildTree(int node, int l, int r, vector<int> &a) {
		lz2[node] = 1;
		if (l == r) {
			tr[node] = a[l] % p;
			return;
		}
		int mid = (l + r) >> 1;
		buildTree(node << 1, l, mid, a);
		buildTree(node << 1 | 1, mid + 1, r, a);
		push_up(node);
	}
	
	void changeAdd(int node, int l, int r, int inc, int lt, int rt) {
		if (lt <= l && rt >= r) {
			tr[node] = (tr[node] + (r - l + 1) * inc) % p;
			lz1[node] = (lz1[node] + inc) % p;
			return;
		}
		push_down(node, l, r);
		int mid = (l + r) >> 1;
		if (lt <= mid) changeAdd(node << 1, l, mid, inc, lt, rt); 
		if (rt > mid) changeAdd(node << 1 | 1, mid + 1, r, inc, lt, rt);
		push_up(node);
	}
	
	void changeMultiply(int node, int l, int r, int inc, int lt, int rt) {
		if (lt <= l && rt >= r) {
			tr[node] = (tr[node] * inc) % p;
			lz1[node] = (lz1[node] * inc) % p;
			lz2[node] = (lz2[node] * inc) % p;
			return;
		}
		push_down(node, l, r);
		int mid = (l + r) >> 1;
		if (lt <= mid) changeMultiply(node << 1, l, mid, inc, lt, rt);
		if (rt > mid) changeMultiply(node << 1 | 1, mid + 1, r, inc, lt, rt);
		push_up(node);
	}
	
	long long querySum(int node, int l, int r, int lt, int rt) {
		if (l >= lt && r <= rt) return tr[node];
		push_down(node, l, r);
		long long ans = 0;
		int mid = (l + r) >> 1;
		if (lt <= mid) ans = (ans + querySum(node << 1, l, mid, lt, rt)) % p;
		if (rt > mid) ans = (ans + querySum(node << 1 | 1, mid + 1, r, lt, rt)) % p;
		return ans;
	}
	
public:
	SegmentTree(vector<int>& a, int p) {
		this->n = a.size() - 1;
		this->p = p;
		tr = lz1 = lz2 = vector<long long> (n * 4);
		buildTree(1, 1, n, a);
	}
	
	void updateAdd(int l, int r, int x) {
		changeAdd(1, 1, n, x % p, l, r);
	}
	
	void updateMultiply(int l, int r, int x) {
		changeMultiply(1, 1, n, x % p, l, r);
	}
	
	long long query(int l, int r) {
		return querySum(1, 1, n, l, r);
	}
	
	void show() {
		cout << '\n';
		for (auto& x : tr) {
			cout << x << " ";
		}
		cout << '\n';
	}
	
	~SegmentTree(){}
};

int n, q, p, x, y, k, op;

int main() {
	n = read(), q = read(), p = read();
	vector<int> a(n + 1);
	for (int i = 1; i <= n; i++) {
		a[i] = read();
	}
	SegmentTree st(a, p);
	while (q--) {
		op = read(), x = read(), y = read();
		if (op == 1) {
			k = read();
			st.updateMultiply(x, y, k);
		}
		if (op == 2) {
			k = read();
			st.updateAdd(x, y, k);
		}
		if (op == 3) {
			printf("%lld\n", st.query(x, y));
		}
		//st.show();
	}
}
