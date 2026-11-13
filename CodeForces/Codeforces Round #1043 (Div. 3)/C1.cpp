#include <bits/stdc++.h>
#include <conio.h>
#define ll long long
#define pii pair<ll, ll>
using namespace std;
const int N = 1e6 + 10;
inline ll read() {
	ll ans = 0, f = 1;
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

long long n;
ll base[20], num[20];
ll solve() {
	n = read();
	vector<ll> vec;
	ll ans = 0;
	while (n) {
		vec.push_back(n % 3);
		n /= 3;
	}
	for (int x = 0; x < vec.size(); x++) {
		ans += vec[x] * num[x];
	}
	return ans;
}

int main() {
	base[0] = 1;
	for (long long i = 1; i <= 19; i++) {
		base[i] = 3ll * base[i - 1];
		num[i] = 3ll * base[i] + i * base[i - 1];
	}
	int t = 1;
	cin >> t;
	while (t--) cout << solve() << '\n';
	return 0;
}
