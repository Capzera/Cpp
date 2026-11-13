#include <bits/stdc++.h>
using namespace std;
long long n, k;
string mp = "0123456789ABCDEFGHJK";
string ans;
int main() {
	cin >> n >> k;
	cout << n << "=";
	while (n) {
		int rest = n % k;
		n /= k;
		if (rest >= 0) ans += mp[rest];
		else ans += mp[rest - k], n++;
	}
	reverse(ans.begin(), ans.end());
	cout << ans << "(base" << k << ")"; 
	return 0;
}
