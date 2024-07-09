#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, m, x, y, z;
	cin >> n;
	cin >> m;
	vector<vector<int> > grid(n, vector<int> (m));
	for (auto& i : grid) {
		for (auto& x : i) cin >> x;
	}
	cin >> x;
	cin >> y;
	cout << grid[x][y];
	return 0;
}
