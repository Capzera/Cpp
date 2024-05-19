#include <bits/stdc++.h>
using namespace std;
int main() {
	vector<int> base10 = {6, 6, 2, 6, 4, 4, 4, 8, 4, 6}, base2 = {6, 8, 4, 2};
	int n, ans = 1;
	cin >> n;
	if (n == 1) {
		cout << 1;
		return 0;
	}
	while (n) {
		ans *= base10[n % 10];
		n /= 5;
		ans = ans * base2[n % 4] % 10;
	}
	cout << ans;
	return 0;
}

