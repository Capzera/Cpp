#include <bits/stdc++.h>
#include <conio.h>
#define ll long long
#define pii pair<ll, ll>
using namespace std;
const int N = 1e6 + 10;
inline int read() {
	int ans = 0, f = 1;
	char ch = getchar();
	while (!isdigit(ch)) {
		if (ch == '-') f = -1;
		ch = getchar();
	}
	while (isdigit(ch)) {
		ans = (ans << 3) + (ans << 1) + ch - '0';
		ch = getchar();
	}
	return f * ans;
}
ll n;
void solve() {
	cin >> n;
	set<ll> s;
	ll base = 10;
	for (int k = 1; k <= 18; k++) {
		if (base + 1 > n) break;
		if (n % (base + 1) == 0) {
			s.insert(n / (base + 1));
		}
		base *= 10;
	}
	cout << s.size() << '\n';
	if (s.empty()) return;
	vector<ll> vec(s.begin(), s.end());
	sort(vec.begin(), vec.end());
	for (ll x : vec) cout << x << " ";
	cout << '\n';
}

int main() {
	int t = 1;
	cin >> t;
	while (t--) solve();
	// while(t--) cout << (solve() ? "Yes\n" : "No\n");
	return 0;
}
