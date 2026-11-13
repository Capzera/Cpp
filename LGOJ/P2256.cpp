#include <bits/stdc++.h>
using namespace std;
struct dsu {
	vector<int> pa, size, dis;
	int cnt;
	dsu(int n) {
		size.resize(n + 1, 1);
		dis = pa = vector<int> (n + 1);
		iota(pa.begin(), pa.end(), 0);
		cnt = n;
	}
	int find(int x) {
		if (x == pa[x]) return x;
		int root = find(x);
		dis[x] += dis[pa[x]];
		return pa[x] = root;
	}
	void merge(int x, int y) {
		x = find(x), y = find(y);
		if (x == y) return;
		if (size[x] < size[y]) swap(x, y);
		pa[y] = x;
		size[x] += size[y];
		cnt--;
	}
	bool isConnected(int x, int y) {
		return find(x) == find(y);
	}
};

int n, m, k, id;
string u, v;
map<string, int> mp;
int main() {
	cin >> n >> m;
	struct dsu uf(n);
	while (n--) {
		cin >> u;
		mp[u] = id++;
	}
	while (m--) {
		cin >> u >> v;
		uf.merge(mp[u], mp[v]);
	}
	cin >> k;
	while (k--) {
		cin >> u >> v;
		cout << (uf.isConnected(mp[u], mp[v]) ? "Yes.\n" : "No.\n");
	}
	return 0;
}
