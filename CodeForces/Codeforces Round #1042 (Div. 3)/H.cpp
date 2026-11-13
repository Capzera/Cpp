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

void solve() {
	
}

int main() {
	int t = 1;
	t = read();
	while (t--) solve();
	// while(t--) cout << (solve() ? "Yes\n" : "No\n");
	return 0;
}
