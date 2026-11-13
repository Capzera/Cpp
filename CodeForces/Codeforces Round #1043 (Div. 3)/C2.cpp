#include <bits/stdc++.h>
#include <conio.h>
#define ll long long
using namespace std;
const int N = 1e6 + 10;
inline ll read() {
	ll ans = 0;
	char ch = getchar();
	while (!isdigit(ch)) ch = getchar();
	while (isdigit(ch)) {
		ans = (ans << 3) + (ans << 1) + ch - '0';
		ch = getchar();
	}
	return ans;
}
ll n, k, base[25], num[25];
ll solve() {
	n = read(), k = read();
	if (k >= n) return num[0] * n;
	vector<ll> vec;
	ll ans = 0;
	while (n) {
		vec.push_back(n % 3);
		k -= n % 3;
		n /= 3;
	}
	if (k < 0) return -1;
	for (int i = vec.size() - 1; ~i; i--) {
		ll cnt = min(vec[i], k / 2);
		vec[i] -= cnt;
		vec[i - 1] += 3ll * cnt;
		k -= 2ll * cnt;
	}
	for (int x = 0; x < vec.size(); x++) {
		ans += vec[x] * num[x];
	}
	return ans;
}
int main() {
	base[0] = 1;
	num[0] = 3;
	for (ll i = 1; i <= 24; i++) {
		base[i] = 3ll * base[i - 1];
		num[i] = 3ll * base[i] + 1ll * i * base[i - 1];
	}
	int t = 1;
	t = read();
	while (t--) cout << solve() << '\n';
	return 0;
}
