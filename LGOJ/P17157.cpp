#include <bits/stdc++.h>
using namespace std;
int n, t, x, ans;
int main() {
	cin >> n >> t;
	while (n--) {
		cin >> x;
		ans += x == t ? 2 : abs(x - t) <= 2;
	}
	cout << ans;
	return 0;
}
