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
int n, m;
string a, b, c;
void solve() {
	cin >> n >> a >> m >> b >> c;
	for (int i = 0; i < m; i++) {
		if (c[i] == 'V') {
			a = b[i] + a;
		} else {
			a += b[i];
		}
	}
	cout << a << endl;
}

int main() {
	int t = 1;
	cin >> t;
	while (t--) solve();
	// while(t--) cout << (solve() ? "Yes\n" : "No\n");
	return 0;
}
