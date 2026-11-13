#include <bits/stdc++.h>
using namespace std;
long long n, ans;
int main() {
	cin >> n;
	for (long long x = 2; x <= n; x <<= 1) {
		ans += (int)sqrt(n - x);
	}
	cout << ans;
	return 0;
}
