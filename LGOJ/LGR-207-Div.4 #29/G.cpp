#include <bits/stdc++.h>
using namespace std;
int l, r, a[7], ans;
bool check(int x) {
	for (int i = 1; i <= 6; i++) {
		a[i] = x % 10;
		x /= 10;
		if (!a[i]) return 0;
	}
	sort(a + 1, a + 7);
	if (a[1] + a[2] > a[3] && a[4] + a[5] > a[6]) return 1;
	if (a[1] + a[2] > a[4] && a[3] + a[5] > a[6]) return 1;
	if (a[1] + a[2] > a[5] && a[3] + a[4] > a[6]) return 1;
	if (a[1] + a[2] > a[6] && a[3] + a[4] > a[5]) return 1;
	if (a[1] + a[3] > a[4] && a[2] + a[5] > a[6]) return 1;
	if (a[1] + a[3] > a[5] && a[2] + a[4] > a[6]) return 1;
	if (a[1] + a[3] > a[6] && a[2] + a[4] > a[5]) return 1;
	if (a[1] + a[4] > a[5] && a[2] + a[3] > a[6]) return 1;
	if (a[1] + a[4] > a[6] && a[2] + a[3] > a[5]) return 1;
	if (a[1] + a[5] > a[6] && a[2] + a[3] > a[4]) return 1;
	return 0;
}
int main() {
	cin >> l >> r;
	for (int i = l; i <= r; i++) {
		ans += check(i);
	}
	cout << ans;
	return 0;
}
/*
任意两边之和大于第三边
a + b > c && b + c > a && a + c > b
a[1] <= a[2] <= a[3] <= a[4] <= a[5] <= a[6];
C(6, 3) = 20 / 2 = 10

当a <= b <= c
a + b > c
*/
