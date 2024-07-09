#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, ans = -1, sum = 0, total = 0;
	string s;
	cin >> n >> s;
	for (int i = 0; i < n; i++) {
		total += s[i] - '0';
	}
	for (int i = 0; i <= n; i++) {
		if (i - sum >= (i + 1) / 2 && total - sum >= (n - i + 1) / 2) {
			if (abs(n / 2.0 - i) < abs(n / 2.0 - ans)) ans = i;
		}
		if (i < n) sum += s[i] - '0';
	}
	cout << ans << endl;
	return 0;
}
/*
(a +b - 1) / b
(a - 1) / b + 1
m >= 1
10 / 4
*/
