#include <bits/stdc++.h>
#include <conio.h>
#define ll long long
#define pii pair<ll, ll>
using namespace std;
const int N = 1e6 + 10;
inline ll read() {
	ll ans = 0;
	char ch = getchar();
	while (!isdigit(ch)) ch = getchar();
	while (isdigit(ch)) {
		ans = ans * 10 + ch - '0';
		ch = getchar();
	}
	return ans;
}
ll f(ll n) {
	ll total = 0;
	ll d = 1;
	ll i = 1;
	while (i <= n) {
		ll j = min(n, i * 10 - 1);
		total += (j - i + 1) * d;
		i = j + 1;
		d++;
	}
	return total;
}

ll count(ll n, int j, int x) {
	if (n == 0) return 0;
	ll mask = 1;
	for (int i = 1; i < j; i++) mask *= 10;
	ll r = n / (mask * 10);
	ll l = n % mask;
	ll cur = (n / mask) % 10;
	if (x == 0) {
		if (r == 0) return 0;
		return (r - 1) * mask + l + 1;
	} else {
		ll count = r * mask;
		if (cur > x) count += mask;
		else if (cur == x) count += l + 1;
		return count;
	}
}

ll query(ll L, ll R, int d) {
	ll total = 0;
	for (int i = 1; i <= d; i++) {
		int j = d - i + 1;
		for (int x = 0; x <= 9; x++) {
			if (i == 1 && x == 0) continue;
			ll cnt = count(R, j, x) - count(L - 1, j, x);
			total += x * cnt;
		}
	}
	return total;
}

ll get(ll n) {
	if (n == 0) return 0;
	ll total_sum = 0;
	ll d = 1;
	ll i = 1;
	while (i <= n) {
		ll j = min(n, i * 10 - 1);
		if (i == (ll)pow(10, d - 1) && j == (ll)pow(10, d) - 1) {
			total_sum += 1ll * 45 * pow(10, d - 1) + 1ll * 405 * (d - 1) * pow(10, d - 2);
		} else {
			total_sum += query(i, j, d);
		}
		i = j + 1;
		d++;
	}
	return total_sum;
}
ll n;
void solve() {
	ll k;
	cin >> k;
	ll l = 1, r = 1e18;
	ll n = 0;
	while (l <= r) {
		ll mid = (l + r) / 2;
		if (f(mid) <= k) {
			n = mid;
			l = mid + 1;
		} else {
			r = mid - 1;
		}
	}
	ll total = get(n);
	ll rem = k - f(n);
	if (rem > 0) {
		string next_num = to_string(n + 1);
		for (int i = 0; i < rem; i++) {
			total += next_num[i] - '0';
		}
	}
	cout << total << '\n';
}

int main() {
	int t = 1;
	cin >> t;
	while (t--) solve();
	// while(t--) cout << (solve() ? "Yes\n" : "No\n");
	return 0;
}
