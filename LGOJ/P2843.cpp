#include <bits/stdc++.h>
using namespace std;
int n, x, k, ans, a[31];
map<vector<int>, int> mp;
int main() {
	cin >> n >> k;
	vector<int> diff(k - 1, 0);
	mp[diff] = 0;
	for (int i = 1; i <= n; i++) {
		cin >> x;
		for (int j = 1; j <= k; j++) {
			a[j] = a[j] + (x & 1);
			x >>= 1;
		}
		for (int j = 2; j <= k; j++) {
			diff[j - 2] = a[j] - a[j - 1];
		}
		if (mp.count(diff)) {
			ans = max(ans, i - mp[diff]);
		} else mp[diff] = i;
	}
	cout << ans;
	return 0;
}
/*
41
平局 = 2
胜利 = 3
2x + 3y = 41
x = 19 y = 1
x = 16 y = 3
x = 13 y = 5
x = 10 y = 7
x = 7  y = 9
x = 4  y = 11
x = 1  y = 13
111  111
110  221
111  332
010  342
001  343
100  443
010  453
0 0
1 0
1 0
2 -1
1 -1
1 0
2 -1
0 0
1 1
1 1


a[i][j] =
*/
