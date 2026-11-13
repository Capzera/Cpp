#include <bits/stdc++.h>
using namespace std;
int n, k, ans = 0;
void dfs(int i, int sum) {
	if (sum != 0) ans++;
	if (i == n) return;
	int st = 0;
	if (i == 0) st = 1;
	for (int j = st; j <= 9; j++) {
		if (sum + j > k) return;
		dfs(i + 1, sum + j);
	}
}
int main() {
	cin >> n >> k;
	dfs(0, 0);
	cout << ans << endl;
	return 0;
}
