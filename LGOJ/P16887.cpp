#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10;
int t, n, pi[N];
bool vis[N];
unordered_map<int, int> mp;
string s;
string solve() {
	cin >> n >> s;
	s = " " + s;
	memset(vis, 0, sizeof(vis));
	for (int i = 2, j = 0; i <= n; i++) {
		while (j && s[i] != s[j + 1]) j = pi[j];
		if (s[i] == s[j + 1]) j++;
		pi[i] = j;
	}
	for (int i = n; i > 0; i = pi[i]) vis[i] = 1;
	vis[0] = 1;
	for (int i = 1; i <= n; i++) {
		if (!vis[i] ||!vis[n - i]) continue;
		return s.substr(1, i);
	}
	return "";
}
int main() {
	cin >> t;
	for (int i = 1; i <= t; i++) {
		cout << "Case #" << i << ": " << solve() << '\n';
	}
	return 0;
}
