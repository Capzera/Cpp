#include <bits/stdc++.h>
using namespace std;
long long n, m, ans;
int main() {
	cin >> n >> m;
	vector<vector<int> > a(n + 1, vector<int>(2));
	for (int i = 1; i <= n; i++) {
		cin >> a[i][0] >> a[i][1];
	}
	sort(a.begin(), a.end());
	for (int i = 1; i <= n; i++) {
		ans += max(a[i][1] - a[i - 1][1], 0);
	}
	cout << ans << endl;
	return 0;
}
