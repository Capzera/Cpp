#include <bits/stdc++.h>
using namespace std;
const int N = 100, p1 = 131, m1 = 998244353;
string s1, s2;
int n1, n2, ans;
long long h1[N], h2[N], p[N];
long long getHash(long long *a, int l, int r) {
	return (a[r] - a[l - 1] * p[r - l + 1] % m1 + m1) % m1;
}
int main() {
	cin >> s1 >> s2;
	n1 = s1.size(), n2 = s2.size();
	p[0] = 1;
	for (int i = 1; i <= max(n1, n2); i++) {
		p[i] = p[i - 1] * p1 % m1;
	}
	for (int i = 1; i <= n1; i++) {
		h1[i] = (h1[i - 1] * p1 + s1[i - 1]) % m1;
	}
	for (int i = 1; i <= n2; i++) {
		h2[i] = (h2[i - 1] * p1 + s2[i - 1]) % m1;
	}
	for (int len = min(n1, n2); len > 0; len--) {
		if (getHash(h1, 1, len) == getHash(h2, n2 - len + 1, n2) || 
			getHash(h2, 1, len) == getHash(h1, n1 - len + 1, n1)) {
			ans = len;
			break;
		}
	}
	cout << ans;
	return 0;
}
