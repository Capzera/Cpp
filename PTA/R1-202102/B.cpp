#include <bits/stdc++.h>
using namespace std;
long long n, x, ans;
int main() {
	cin >> n >> x;
	for (int i = 1; i <= n; i++) {
		int j = i;
		while (j) {
			ans += j % 10 == x;
			j /= 10;
		}
	}
	cout << ans;
	return 0;
}
