#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10;
vector<int> mp[N];
long long ans, mx, n, x;
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> x;
		for (int j = 2; j * j <= x; j++) {
			if (x % j) continue;
			int cnt = 0;	
			while (x % j == 0) {
				cnt++;
				x /= j;
			}
			mp[j].push_back(cnt);
		}
		if (x > 1) mp[x].push_back(1);
	}
	for (int i = 2; i <= 1e5; i++) {
		if (mp[i].size() == 0) continue;
		sort(mp[i].begin(), mp[i].end());
		int m = n - mp[i].size(), mid = 0;
		if (m <= n / 2) mid = mp[i][n / 2 - m];
		for (auto& x : mp[i]) ans += abs(mid - x);
		ans += mid * m;
	}
	cout << ans;
	return 0;
}
