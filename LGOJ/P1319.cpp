#include <bits/stdc++.h>
#define int long long
using namespace std;
int n, p, a;
string ans;
signed main() {
	cin >> n;
	while (cin >> a) {
		string t(a, '0' + p);
		ans += t;
		p ^= 1;
	}
	for (int i = 0; i < n * n; i++) {
		cout << ans[i];
		if (i % n == n - 1) cout << endl;
	}
	return 0;
}
