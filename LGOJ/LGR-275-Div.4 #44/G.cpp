#include <bits/stdc++.h>
using namespace std;
int t, n;
string s;
unordered_set<char> mp = {'a', 'e', 'i', 'u', 'o'};
void solve() {
	cin >> s;
	cout << char(s[0] & -33);
	for (int i = 0; i < s.size() - 1; i++) {
		if (mp.count(s[i])) cout << char(s[i + 1] & -33);
	}
	puts("");
}
int main() {
	cin >> t;
	while (t--) solve();
	return 0;
}
