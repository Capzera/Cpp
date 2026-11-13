#include <bits/stdc++.h>
using namespace std;
long long n, m, t, k;
vector<int> ans;
map<string, int> mp;
struct team {
	string s[4];
	long long id, r;
	bool operator < (const struct team& rhs) {
		if (r != rhs.r) return r < rhs.r;
		return id < rhs.id;
	}
} a[10010];
int main() {
	cin >> n >> m >> t >> k;
	for (int i = 1; i <= n; i++) {
		for (int j = 0; j < 4; j++) cin >> a[i].s[j];
		cin >> a[i].id >> a[i].r;
	}
	sort(a + 1, a + n + 1);
	for (int i = 1; k && i <= n; i++) {
		bool f = 1;
		for (int j = 1; j < 4; j++) {
			if (!mp.count(a[i].s[j])) continue;
			f = 0;
			break;
		}
		if (f) {
			for (int j = 1; j < 4; j++) mp[a[i].s[j]]++;
			ans.push_back(i), k--;
		}
	}
	cout << ans.size() << endl;
	for (auto& x : ans) {
		for (int j = 0; j < 4; j++) cout << a[x].s[j] << " ";
		cout << endl;
	}
	return 0;
}
