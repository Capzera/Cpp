#include <bits/stdc++.h>
using namespace std;
long long n, x, ans;
int main() {
	cin >> n;
	while (n--) {
		cin >> x;
		while (x) {
			ans += x & 1;
			x >>= 1;
		}
	}
	cout << ans << " " << (ans & 1) << endl;
	return 0;
}
