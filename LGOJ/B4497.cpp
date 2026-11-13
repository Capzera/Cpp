#include <bits/stdc++.h>
using namespace std;
int l, r, ans;
int main() {
	cin >> l >> r;
	for (int i = l; i <= r; i++) {
		int n = i, cnt = 0;
		while (n) {
			cnt += n % 10 == 2;
			n /= 10;
		}
		ans += cnt == 3;
	}
	cout << ans;
	return 0;
}
