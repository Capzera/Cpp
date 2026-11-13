#include <bits/stdc++.h>
using namespace std;
const int N = 2e5 + 5;
long long n, a[N], ans;
char s[N];
int main() {
	// freopen("string.in", "r", stdin);
	// freopen("string.out", "w", stdout);
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i] >> s[i];
	for (int l = 1, r = n; l < r; l += !a[l], r -= !a[r]) {
		int rem = min(a[l], a[r]);
		ans += (s[l] != s[r]) * rem;
		a[l] -= rem, a[r] -= rem;
	}
	cout << ans;
	return 0;
}


