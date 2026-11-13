#include <bits/stdc++.h>
using namespace std;
long long n, ans;
int main() {
	cin >> n;
	for (int i = 1, j = 1, cnt = 0; i <= n; i++) {
		ans += j;
		if (++cnt == j) {
			cnt = 0, j++;
		}
	}
	cout << ans;
	return 0;
}
