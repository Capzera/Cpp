#include <bits/stdc++.h>
using namespace std;
char a[5][5];
int t;

int solve() {
	vector<int> rx(4), cx(4), ro(4), co(4);
	int d1r = 0, d2r = 0, d1o = 0, d2o = 0, f = 1;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			cin >> a[i][j];
			if (a[i][j] == 'X' || a[i][j] == 'T') {
				rx[i]++, cx[j]++;
				if (i == j) d1r++;
				if (i + j == 3) d2r++;
			}
			if (a[i][j] == 'O' || a[i][j] == 'T') {
				ro[i]++, co[j]++;
				if (i == j) d1o++;
				if (i + j == 3) d2o++;
			}
			if (a[i][j] == '.') f = 0;
		}
	}
	if (d1r == 4 || d2r == 4) return 1;
	if (d1o == 4 || d2o == 4) return 2;
	for (int i = 0; i < 4; i++) {
		if (rx[i] == 4 || cx[i] == 4) return 1;
		if (ro[i] == 4 || co[i] == 4) return 2;
	}
	if (f) return 3;
	return 4;
}

int main() {
	cin >> t;
	for (int i = 1; i <= t; i++) {
		int ret = solve();
		cout << "Case #" << i << ": ";
		if (ret == 1) cout << "X won\n";
		if (ret == 2) cout << "O won\n";
		if (ret == 3) cout << "Draw\n";
		if (ret == 4) cout << "Game has not completed\n";
	}
	return 0;
}
