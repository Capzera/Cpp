#include <bits/stdc++.h>
using namespace std;
long long n, x, ans;
int main() {
	cin >> n;
	while (n--) {
		scanf("%lld", &x);
		ans ^= x;
	}
	cout << ans;
	return 0;
}
