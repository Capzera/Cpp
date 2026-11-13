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
	ll m = n, sum = 0;
	while (m) {
		vec.push_back(m % 3);
		sum += m % 3;
		m /= 3;
	}
	if (sum > k) return -1;
	ll rest = k - sum, ans = 0, s = n - k; // 有几次机会可以放缩
	for (int i = 20; i >= 1; i--) {
		ll rem = base[i] - 1;
		if (rem > s) continue;
		ll cnt = s / rem;
		ans += cnt * i * base[i - 1];
		s %= rem;
		if (!s) break;
	}
	if (s > 0) ans++;
	return 3ll * n + ans;
}
int main() {
	base[0] = 1;
	num[0] = 3;
	for (ll i = 1; i <= 24; i++) {
		base[i] = 3ll * base[i - 1];
		num[i] = 3ll * base[i] + i * base[i - 1];
	}
	int t = 1;
	t = read();
	while (t--) cout << solve() << '\n';
	return 0;
}
