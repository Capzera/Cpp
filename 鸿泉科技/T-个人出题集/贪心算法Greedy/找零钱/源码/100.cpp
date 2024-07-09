#include <bits/stdc++.h>
using namespace std;
int main() {
	int money, ans = 0;
	cin >> money;
	vector<int> a(7), v = {1, 2, 5, 10, 20, 50, 100};
	for (int i = 0; i < 7; i++) cin >> a[i];
	for (int i = 6; money > 0 && i >= 0; i--) {
		int cnt = min((money + v[i] - 1) / v[i], a[i]);
		money -= cnt * v[i];
		ans += cnt;
	}
	if (money > 0) ans = -1;
	cout << ans << endl;
	return 0;
}
