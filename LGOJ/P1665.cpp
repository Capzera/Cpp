#include <bits/stdc++.h>
using namespace std;
const int N = 505;
int n, cnt, ans, nx[N], ny[N], loc[5];
map<pair<int, int>, int> mp;
void dfs(int i, int lst) {
	if (i == 4) {
		int x1 = nx[loc[1]], y1 = ny[loc[1]];
		int x2 = nx[loc[2]], y2 = ny[loc[2]];
		int x3 = nx[loc[3]], y3 = ny[loc[3]];
		int x4, y4;
		int d1 = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
		int d2 = (x1 - x3) * (x1 - x3) + (y1 - y3) * (y1 - y3);
		int d3 = (x2 - x3) * (x2 - x3) + (y2 - y3) * (y2 - y3);
		if (d1 == d2 && d1 && d3 == 2 * d1) {
			x4 = x2 + x3 - x1, y4 = y2 + y3 - y1;
			if (mp.count({x4, y4})) ans++;
		} else if (d1 == d3 && d1 && d2 == 2 * d1) {
			x4 = x1 + x3 - x2, y4 = y1 + y3 - y2;
			if (mp.count({x4, y4})) ans++;
		} else if (d2 == d3 && d2 && d1 == 2 * d2) {
			x4 = x2 + x1 - x3, y4 = y2 + y1 - y3;
			if (mp.count({x4, y4})) ans++;
		}
		return;
	}
	for (int j = lst + 1; j <= n; j++) {
		loc[i] = j;
		dfs(i + 1, j);
	}
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> nx[i] >> ny[i];
		mp[{nx[i], ny[i]}] = 1;
	}
	dfs(1, 0);
	cout << ans / 4;
	return 0;
}
