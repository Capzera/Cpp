#include <bits/stdc++.h>
using namespace std;
const long long p = 131, mod = 998244353;
long long l1, l2, ans;
string s1, s2;
vector<long long> base, h1, h2;
long long get(vector<long long>& hash, int l, int r) {
	return (hash[r] - hash[l - 1] * base[r - l + 1] % mod + mod) % mod;
}
int main() {
	cin >> s1 >> s2;
	l1 = s1.size(), l2 = s2.size();
	base.resize(max(l1, l2) + 1), h1.resize(l1 + 1), h2.resize(l2 + 1);
	base[0] = 1;
	for (int i = 1; i <= max(l1, l2); i++) base[i] = base[i - 1] * p % mod;
	for (int i = 0; i < l1; i++) h1[i + 1] = (h1[i] * p + s1[i]) % mod;
	for (int i = 0; i < l2; i++) h2[i + 1] = (h2[i] * p + s2[i]) % mod;
	for (int i = 1; i <= min(l1, l2); i++) {
		if (get(h1, 1, i) == get(h2, l2 - i + 1, l2)) ans = max(ans, 1ll * i);
		if (get(h2, 1, i) == get(h1, l1 - i + 1, l1)) ans = max(ans, 1ll * i);
	}
	cout << ans;
	return 0;
}
