#include <bits/stdc++.h>
using namespace std;
struct dsu {
	vector<int> pa, dis, size;
	int cnt;
	dsu(int n) {
		size.resize(n + 1, 1);
		dis = pa = vector<int> (n + 1);
		iota(pa.begin(), pa.end(), 0);
		cnt = n;
	}
	int find(int x) {
		if (x == pa[x]) return x;
		int root = find(pa[x]);
		dis[x] += dis[pa[x]];
		return pa[x] = root;
	}
	void merge(int x, int y) {
		x = find(x), y = find(y);
		if (x == y) return;
		dis[x] += size[y];
		pa[x] = y;
		size[y] += size[x];
		cnt--;
	}	
	bool isConnected(int x, int y) {
		return find(x) == find(y);
	}
};

int n, u, v;
char op;
int main() {
	cin >> n;
	struct dsu uf(n);
	while (n--) {
		cin >> op >> u >> v;
		if (op == 'M') {
			uf.merge(u, v);
			continue;
		}
		if (!uf.isConnected(u, v)) cout << -1 << endl;
		else cout << abs(uf.dis[u] - uf.dis[v]) - 1 << endl;
		
	}
	return 0;
}
