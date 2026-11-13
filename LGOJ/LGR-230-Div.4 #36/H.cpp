#include <bits/stdc++.h>
using namespace std;
long long n, m, c, k, r1, c1, r2, c2, p, rt, ct, imp[501][501], sticker[501][501];
map<int, pair<int, int> > mp;
int main() {
	cin >> n >> m >> c >> k;
	for (int i = 1; i <= k; i++) {
		cin >> r1 >> c1 >> r2 >> c2 >> p >> rt >> ct;
		mp[p] = {rt - r1, ct - c1};
		for (int x = r1; x <= r2; x++)
			for (int y = c1; y <= c2; y++)
				imp[x][y] = max(imp[x][y], p);
	}
	for (int i = 1; i <= c; i++)
		for (int j = 1; j <= c; j++)
			cin >> sticker[i][j];
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			if (!imp[i][j]) {
				cout << -1 << " ";
				continue;
			}
			auto [dx, dy] = mp[imp[i][j]];
			cout << sticker[dx + i][dy + j] << " ";
		}
		cout << endl;
	}
	return 0;
}
