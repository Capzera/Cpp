#include <bits/stdc++.h>
using namespace std;
class UnionFind {
private:
	vector<int> pa, size;
	int count;
public:
	UnionFind(int n) {
		pa = size = vector<int>(n + 1, 1);
		iota(pa.begin(), pa.end(), 0);
	}
	int find(int x) {
		return pa[x] == x ? x : pa[x] = find(pa[x]);
	}
	void merge(int x, int y) {
		x = find(x), y = find(y);
		if (x == y) return;
		if (size[x] < size[y]) swap(x, y);
		pa[y] = x;
		size[x] += size[y];
		count--;
	}
	bool check() {
		return count == 1;
	}
	bool isConnected(int x, int y) {
		x = find(x), y = find(y);
		return x == y;
	}
};
int n, m, k, ans, cnt = 1;
string s1, s2, s3;
unordered_map<string, int> mp;
int main() {
	UnionFind uf(300001);
	cin >> n;
	vector<vector<int> > a(n + 1);
	for (int i = 1; i <= n; i++) {
		cin >> s1 >> s2;
		if (!mp.count(s1)) mp[s1] = cnt++;
		if (!mp.count(s2)) mp[s2] = cnt++;
		a[i] = {mp[s1], mp[s2]};
	}
	cin >> m;
	vector<vector<int> > b(m + 1);
	for (int i = 1; i <= m; i++) {
		cin >> s1 >> s2;
		if (!mp.count(s1)) mp[s1] = cnt++;
		if (!mp.count(s2)) mp[s2] = cnt++;
		b[i] = {mp[s1], mp[s2]};
	}
	cin >> k;
	for (int i = 1; i <= k; i++) {
		cin >> s1 >> s2 >> s3;
		if (!mp.count(s1)) mp[s1] = cnt++;
		if (!mp.count(s2)) mp[s2] = cnt++;
		if (!mp.count(s3)) mp[s3] = cnt++;
		uf.merge(mp[s1], mp[s2]);
		uf.merge(mp[s2], mp[s3]);
	}
	for (int i = 1; i <= n; i++) {
		ans += !uf.isConnected(a[i][0], a[i][1]);
	}
	for (int i = 1; i <= m; i++) {
		ans += uf.isConnected(b[i][0], b[i][1]);
	}
	cout << ans;
	return 0;
}
