#include <bits/stdc++.h>
using namespace std;
int n, k, ans = INT_MAX, a[101];
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
		if (!a[i]) ans = min(ans, i);
	}
	cin >> k;
	if (!a[k]) ans = k;
	cout << ans;
	return 0;
}
