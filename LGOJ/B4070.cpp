#include <bits/stdc++.h>
using namespace std;
long long n, ans;
int main() {
	cin >> n;
	int m = sqrt(n);
	for (int i = 2; i <= m; i++) {
		int cnt = 0;
		while (n % i == 0) {
			cnt++;
			n /= i;
		}
		ans += (sqrt(8 * cnt + 1) - 1) / 2;
	}
	cout << ans + (n != 1);
	return 0;
}
