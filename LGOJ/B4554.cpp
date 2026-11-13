#include <bits/stdc++.h>
using namespace std;
int n;
int main() {
	cin >> n;
	vector<string> ans(2 * n - 1, string(2 * n - 1, '.'));
	for (int i = 0; i < n; i++) {
		ans[i][n - i - 1] = ans[n - 1 + i][i] = ans[i][n - 1 + i] = ans[n - 1 + i][2 * n - 2 - i] = '+';
	}
	for (auto& x : ans) cout << x << '\n';
	return 0;
}



