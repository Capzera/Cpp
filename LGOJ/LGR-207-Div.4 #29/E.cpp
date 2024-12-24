#include <bits/stdc++.h>
using namespace std;
long long n, ans[6], x, m;
int main() {
	cin >> m >> n;
	while (n--) {
		long long mx = 0;
		for (int i = 1; i <= 5; i++) {
			cin >> x;
			if (x <= m) mx = max(mx, x);
		}
		m -= mx;
		ans[mx]++;
	}
	for (int i = 1; i <= 5; i++) cout << ans[i] << " ";
	cout << endl << m;
	return 0;
}
