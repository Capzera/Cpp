#include <bits/stdc++.h>
using namespace std;
int T;
string s1, s2;
int solve() {
	cin >> s2 >> s1;
	int l1 = s1.size(), l2 = s2.size();
	if (l1 < l2 || l1 == l2 && s1 != s2) return -1;
	for (int i = 0, j = 0; i < l1; i++) {
		if (s1[i] == s2[j]) j++;
		if (j == l2) return l1 - l2;
	}
	return -1;
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> T;
	for (int i = 1; i <= T; i++) {
		cout << "Case #" << i << ": ";
		int ans = solve();
		cout << (ans == -1 ? "IMPOSSIBLE" : to_string(ans)) << '\n';
	}
	return 0;
}

