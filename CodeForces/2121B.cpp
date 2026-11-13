#include <bits/stdc++.h>
using namespace std;
string s;
int n;
int solve() {
	cin >> n >> s;
	vector<int> mp(26);
	for (auto& ch : s) mp[ch - 'a']++;
	for (int i = 1; i < n - 1; i++) {
		if (mp[s[i] - 'a'] > 1) return 1;
	}
	return 0;
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cout.tie(nullptr)->sync_with_stdio(0);
	int t;
	cin >> t;
	while (t--) cout << (solve() ? "Yes\n" : "No\n");
	return 0;
}

