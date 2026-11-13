#include <bits/stdc++.h>
using namespace std;
int n, ans, vis[100001], a[100001];
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i];
	sort(a + 1, a + n + 1);
	int l = n / 2, r = n;
	while (l) {
		if (a[r] >= a[l] * 2) {
			ans++;
			vis[l] = vis[r] = 1;
		}
		l--;
		while (r && vis[r]) r--;
	}
	cout << ans;
	return 0;
}
