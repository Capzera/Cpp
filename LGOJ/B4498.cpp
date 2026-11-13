#include <bits/stdc++.h>
using namespace std;
int n;
int main() {
	cin >> n;
	vector<string> ans(n, string(n, '*'));
	for (int j = 1; j < n - 1; j++) ans[0][j] = ans[n - 1][j] = '-';
	for (int i = 1; i < n - 1; i++) ans[i][0] = ans[i][n - 1] = '|';
	ans[0][0] = ans[0][n - 1] = ans[n - 1][0] = ans[n - 1][n - 1] = '+';
	for (auto& x : ans) cout << x << '\n';
	return 0;
}
