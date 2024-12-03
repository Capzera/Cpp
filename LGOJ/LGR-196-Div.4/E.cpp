#include <bits/stdc++.h>
using namespace std;
long long n, x, t, ans;
bool mp[10001];
int main() {
	cin >> n >> t;
	t = pow(10, t);
	while (n--) {
		cin >> x;
		mp[x % t] = 1;
	}
	for (int i = 0; i < t; i++) {
		ans += mp[i];
	}
	cout << ans;
	return 0;
}
