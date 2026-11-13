#include <bits/stdc++.h>
using namespace std;
string s;
int n;
int main() {
	cin >> s;
	n = s.size();
	vector<string> ans(6, string(n << 2 | 1, '.'));
	for (int j = 2; j <= 4 * n + 1; j += 4) {
		ans[1][j] = ans[5][j] = ans[2][j - 1] = ans[2][j + 1] = ans[4][j - 1] = ans[4][j + 1] = ans[3][j - 2] = ans[3][j + 2] = '#';
	}
	for (int i = 0; i < n / 3; i++) {
		int j = 12 * i + 10;
		ans[1][j] = ans[5][j] = ans[2][j - 1] = ans[2][j + 1] = ans[4][j - 1] = ans[4][j + 1] = ans[3][j - 2] = ans[3][j + 2] = '*';
	}
	for (int i = 0; i < n; i++) {
		ans[3][4 * i + 2] = s[i];
	}
	for (int i = 1; i <= 5; i++) cout << ans[i] << '\n';
	return 0;
}
// 10 22 34
//2 6 10 14 18 22 26 30 34
//4x - 2

