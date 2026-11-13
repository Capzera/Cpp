#include <bits/stdc++.h>
using namespace std;
int l, r, ans;
int main() {
	cin >> l >> r;
	for (int i = l; i <= r; i++) {
		int sq = sqrt(i);
		ans += sq * sq == i;
	}
	cout << ans;
	return 0;
}
