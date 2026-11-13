#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, m, k, x, y, h, c;
	cin >> n >> m >> k;
	vector<vector<int> > color(n + 1, vector<int>(m + 1)), height(n + 1, vector<int>(m + 1, INT_MAX));
	for (int i = 0; i < k; i++) {
		cin >> x >> y >> h >> c;
		if (height[x][y] > h) {
			height[x][y] = h;
			color[x][y] = c;
		}
	}
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) cout << color[i][j] << " ";
		cout << endl;
	}
	return 0;
}
