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

void solve() {
	
}

int main() {
	int t = 1;
	cin >> t;
	while (t--) solve();
	// while(t--) cout << (solve() ? "Yes\n" : "No\n");
	return 0;
}
