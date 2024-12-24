#include <bits/stdc++.h>
using namespace std;
int  main() {
	long long n;
	cin >> n;
	vector<long long> a(n + 1);
	for (int i = 1; i <= n; i++) cin >> a[i];
	long long ans = 0, money = 0;
	for (long long i = 1; i <= n; i++) {
		money += a[i] - a[i - 1];
		if (money > 50) money = 50;
		ans += money / 8;
		money %= 8;
	}
	cout << ans << endl;
	return 0;
}
