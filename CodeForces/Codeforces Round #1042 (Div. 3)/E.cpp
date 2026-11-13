#include <bits/stdc++.h>
#define ll long long
#define pii pair<int, ll>
using namespace std;
const int N = 1e6 + 10;
const ll inf = 1 << 60;
inline int read() {
	int ans = 0, f = 1;
	char ch = getchar();
	while (!isdigit(ch)) {
		if (ch == '-') f = -1;
		ch = getchar();
	}
	while (isdigit(ch)) {
		ans = ans * 10 + ch - '0';
		ch = getchar();
	}
	return f * ans;
}
int n, a[N], b[N], c[N];
int solve() {
	n = read();
	for (int i = 1; i <= n; i++) {
		a[i] = read();
	}
	for (int i = 1; i <= n; i++) {
		b[i] = read();
		c[i] = b[i];
	}
	if (a[n] != b[n]) return 0;
	for (int i = n - 1; i > 0; i--) {
		if (a[i] == b[i]) continue;
		if (((a[i] ^ a[i + 1]) != b[i]) && (a[i] ^ c[i + 1]) != b[i]) return 0;
		if ((a[i] ^ a[i + 1]) == b[i]) c[i] = a[i] ^ a[i + 1];
		if ((a[i] ^ c[i + 1]) == b[i]) c[i] = a[i] ^ c[i + 1];
	}
	return 1;
}
int main() {
	int t = 1;
	t = read();
	//while (t--) solve();
	while(t--) cout << (solve() ? "YES\n" : "NO\n");
	return 0;
}
