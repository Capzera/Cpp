#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10;
int n, m, u, v, cnt, pa[N];
int find(int x) {
	return x == pa[x] ? x : pa[x] = find(pa[x]);
}
int main() {
	cin >> n >> m;
	iota(pa + 1, pa + n + 1, 1);
	while (m--) {
		cin >> u >> v;
		u = find(u), v = find(v);
		if (u == v) continue;
		pa[v] = u;
		n--;
	}
	cout << n - 1;
	return 0;
}
