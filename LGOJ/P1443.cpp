#include <bits/stdc++.h>
using namespace std;
int main() {
	int dx[8]={-1, -2, -2, -1, 1, 2, 2, 1};
	int dy[8]={2, 1, -1, -2, 2, 1, -1, -2};
	int n, m, x, y;
	cin >> m;
	cin >> n;
	cin >> x; 
	cin >> y;
	queue<pair<int, int> > q;
	vector<vector<int> > a(m + 1, vector<int>(n + 1, -1));
	q.push({x, y});
	a[x][y] = 0;
	while (q.size()) {
		auto& [x, y] = q.front(); q.pop();
		for (int d = 0; d < 8; d++) {
			int mx = x + dx[d], my = y + dy[d];
			if (mx < 1 || mx > m || my < 1 || my > n || a[mx][my] != -1) continue;
			a[mx][my] = a[x][y] + 1;
			q.push({mx, my});
		}	
	}
	for (int i = 1; i <= m; i++) {
		for (int j = 1; j <= n; j++) cout << a[i][j] << " ";
		cout << endl;
	}
	return 0;
}
