#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, d = 0, x = 0, y = 0;
	cin >> n;
	vector<vector<int> > grid(n, vector<int>(n));
	for (int i = 1; i <= n * n; i++) {
		grid[x][y] = i++;
		if (d == 0) y++;
		if (d == 1) x++;
		if (d == 2) y--;
		if (d == 3) x--;
		if (x == n || x < 0 || y == n || y < 0) {
			d++;
			x = (x + n) % n;
			y = (y + n) % n;
		}
	}
	for (auto& i : grid) {
		for (auto& j : i) cout << j << " ";
		cout << endl;
	}
	return 0;
}
