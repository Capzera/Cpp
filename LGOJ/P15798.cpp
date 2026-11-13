#include <bits/stdc++.h>
using namespace std;
int l, r, ans;
int check(int x) {
	while (x % 10 == 0) x /= 10;
	while (x % 2 == 0) x /= 2;
	while (x % 5 == 0) x /= 5;
	return x == 1;
}
int main() {
	cin >> l >> r;
	for (int i = l ; i <= r; i++) {
		ans += check(i);
	}
	cout << ans;
	return 0;
}
