#include <bits/stdc++.h>
using namespace std;
int k, l, r, ans;
int main() {
	cin >> k >> l >> r;
	for (int i = l; i <= r; i++) {
		if (i % k == 0 || i % 10 == k) ans += i;
	}
	cout << ans;
	return 0;
}
