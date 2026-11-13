#include <bits/stdc++.h>
using namespace std;
long long n, m, ans = 1;
int main() {
	cin >> n >> m;
	for (int x = 1; ; x++) {
		int now = x * n + m, f = 1;
		for (int j = 1; j < n; j++) {
			if (now * n % (n - 1)) {
				f = 0;
				break;
			}
			now = now * n / (n - 1) + m;
		}
		if (f) {
			cout << now;
			return 0;
		}
	}
	return 0;
}
