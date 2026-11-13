#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 5;
int n, m, x, sz, tgt, a[N];
unordered_map<int, int> mp;
vector<int> pos, ans;
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
		mp[a[i]]++;
		if (a[i] != a[i - 1]) {
			sz++;
			pos.push_back(i);
		}
	}
	for (int i = 1; i <= n; i++) {
		if (mp.count(i)) continue;
		tgt = i;
		break;
	}
	if (sz == m) return puts("0"), 0;
	cout << "1\n";
	if (sz > m) {
		tgt = a[pos[m - 1]];
		for (int i = pos[m - 1] + 1; i <= n; i++) ans.push_back(i);
	} else {
		int k = m - sz;
		for (int i = 1; i <= n && k; i++) {
			if (a[i] != a[i - 1]) continue;
			if (k == 1 && i < n && a[i] == a[i + 1]) continue;
			ans.push_back(i);
			if (k > 1 && i < n && a[i] == a[i + 1]) {
				k -= 2;
				i++;
			} else k--;
		}
	}
	cout << ans.size() << ' ';
	for (auto& x : ans) cout << x << " ";
	cout << tgt;
	return 0;
}
