#include <bits/stdc++.h>
using namespace std;
int a, b, m, n, ans, f[365];
int main() {
	cin >> a >> b >> m >> n;
	if (a > m) {
		cout << a;
		return 0;
	}
	f[0] = b - a, f[1] = a, ans = a;
	for (int i = 2; i <= n; i++) {
		f[i] = f[i - 1] + f[i - 2];
		ans += f[i];
		if (f[i] >= m) break; 
	}
	cout << ans;
	return 0;
}
