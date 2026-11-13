#include <bits/stdc++.h>
using namespace std;
long long n, m, r, c, ans;
int main() {
	cin >> n >> m;
	while (n-- && m > 0) {
		cin >> r >> c;
		ans += 2;
		m -= r * c;
	}
	cout << ans;
	return 0;
}
