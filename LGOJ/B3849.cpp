#include <bits/stdc++.h>
using namespace std;
string mp = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ", ans;
int n, k;
int main() {
	cin >> n >> k;
	while (n) {
		ans += mp[n % k];
		n /= k;
	}
	reverse(ans.begin(), ans.end());
	cout << ans;
	return 0;
}
