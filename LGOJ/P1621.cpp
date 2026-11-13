#include <bits/stdc++.h>
using namespace std;
class UnionFind {
public:
	vector<int> pa, size;
	int cnt;
	UnionFind(int n) {
		cnt = 0;
		pa = size = vector<int>(n + 1, 1);
		iota(pa.begin(), pa.end(), 0);
	}
	inline int find(int x) {
		return x == pa[x] ? x : pa[x] = find(pa[x]);
	}
	void merge(int x, int y) {
		x = find(x), y = find(y);
		if (x == y) return;
		if (size[x] < size[y]) swap(x, y);
		size[x] += y;
		pa[y] = x;
		cnt++;
	}
};
int l, r, p, ans;
int st[100001];
vector<int> prime;
int main() {
	cin >> l >> r >> p;
	UnionFind uf(r + 1);
	for (int i = 2; i <= r; i++) {
		if (!st[i]) prime.push_back(i);
		for (auto& x : prime) {
			if (i * x > r) break;
			st[i * x] = 1;
			if (i % x == 0) break;
		}
	}
	for (auto& x : prime) {
		if (x < p) continue;
		int base = x;
		while (base < l) base += x;
		for (int i = base; i <= r; i += x) {
			uf.merge(base, i);
		}
	}
	for (int i = l; i <= r; i++) {
		ans += i == uf.find(i);
	}
	cout << ans;
	return 0;
}
