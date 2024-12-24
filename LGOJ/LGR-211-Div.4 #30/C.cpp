#include <bits/stdc++.h>
using namespace std;
long long n, ans, cnt, x, y;
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> x >> y;
		ans += y ? x : 20;
	}
	cout << ans;
	return 0;
}
