#include <bits/stdc++.h>
using namespace std;
string s, query, u, v;
int m;
unordered_map<string, int> mp;
int main() {
	cin >> s >> m;
	queue<string> q; q.push(s);
	mp[s] = 0;
	while (q.size()) {
		u = q.front(); q.pop();
		int n = u.size();
		for (int i = 0; i < n - 1; i++) {
			for (int j = i + 1; j < n; j++) {
				v = u;
				swap(v[i], v[j]);
				if (mp.count(v)) continue;
				mp[v] = mp[u] + 1, q.push(v);
			}
		}
		for (int i = 0; n > 1 && i < n; i++) {
			v = u;
			v.erase(i, 1);
			if (mp.count(v)) continue;
			mp[v] = mp[u] + 1, q.push(v);
		}
		if (n == s.size()) continue;
		for (int i = 1; i < n; i++) {
			for (char c = u[i - 1] + 1; c < u[i]; c++) {
				v = u;
				v.insert(i, 1, c);
				if (mp.count(v)) continue;
				mp[v] = mp[u] + 1, q.push(v);
			}
		}
	}
	while (m--) {
		cin >> query;
		cout << (mp.count(query) ? mp[query] : -1) << endl;
	}
	return 0;
}
