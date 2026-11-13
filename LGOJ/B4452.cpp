#include <bits/stdc++.h>
using namespace std;

struct goods {
	string s;
	int pri, id;
	bool operator < (const struct goods& rhs) const {
		if (id != rhs.id) return id < rhs.id;
		if (pri != rhs.pri) return pri < rhs.pri;
		return s < rhs.s;
	}
} a[100001];

long long m, n;
int main() {
	cin >> m >> n;
	for (int i = 1; i <= n; i++) {
		cin >> a[i].s >> a[i].pri >> a[i].id;
	}
	vector<string> ans;
	sort(a + 1, a + n + 1);
	for (int i = 1; i <= n; i++) {
		if (a[i].pri > m) continue;
		ans.push_back(a[i].s);
		m -= a[i].pri;
	}
	sort(ans.begin(), ans.end());
	for (auto& x : ans) cout << x << '\n';
	return 0;
}
