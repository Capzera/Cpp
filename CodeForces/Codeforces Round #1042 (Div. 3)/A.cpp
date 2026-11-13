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
int n, a[N], b[N];
void solve() {
	n = read();
	int ans = 0;
	for (int i = 1; i <= n; i++) {
		a[i] = read();
	}
	for (int i = 1; i <= n; i++) {
		b[i] = read();
		if (a[i] > b[i]) ans += a[i] - b[i];
	}
	cout << ans + 1 << '\n';
}


int main() {
	int t = 1;
	t = read();
	while (t--) solve();
	// while(t--) cout << (solve() ? "Yes\n" : "No\n");
	return 0;
}
-1 2
-1 3 -1
-1 3 -1 2
-1 3 -1 3 -1
-1 3 -1 3 -1 2
-1 3 -1 3 -1 3 -1
-1 3 -1 3 -1 3 -1 2
3 5
a[i]
b[i]
a[i] + k * m = b[i]
a[i] - b[i] = k * M
a[i] - b[i] % k == 0
a[i] \euiqv b[i]  (mod k)
3 5

