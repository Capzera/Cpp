#include <bits/stdc++.h>
using namespace std;
long long n, x, ans;
int solve() {
	cin >> n >> ans;
	for (int i = 2; i <= n; i++)  {
		cin >> x;
		ans += min(2ll, x); // 行元素可能小于2
		for (int j = 2; j < x; j++) {
			ans += i * j % 10 == 6;
		}
	}
	return ans;
}
int main() {
	int t;
	cin >> t;
	while (t--) cout << solve() << '\n';
	return 0;
}
