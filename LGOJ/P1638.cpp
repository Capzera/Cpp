#include <bits/stdc++.h>
using namespace std;
int n, m, a[1000010], mn = 1e6, l, r;
unordered_map<int, int> mp;
int main() {
	cin >> n >> m;
	for (int i = 1, j = 1; i <= n; i++) {
		cin >> a[i];
		mp[a[i]]++;
		if (mp.size() < m) continue;
		while (mp[a[j]] > 1) {
			mp[a[j++]]--;
		}
		if (i - j + 1 < mn) {
			mn = i - j + 1;
			l = j, r = i;
		}
	}
	cout << l << " " << r;
	return 0;
}
