#include <bits/stdc++.h>
using namespace std;
int l, r, ans;
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> l >> r;
	for (int i = l; i <= r; i++) {
		int j = i;
		while (j) {
			ans += j % 10 == 2;
			j /= 10;
		}
	}
	cout << ans;
	return 0;
}
