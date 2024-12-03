#include <bits/stdc++.h>
using namespace std;
string s, t, tmp;
int n, m, ans, loc = 1e9;
int main() {
	getline(cin, t);
	m = t.size();
	getline(cin, s);
	for (auto& x : t) x = tolower(x);
	for (auto& x : s) x = tolower(x);
	s += " ";
	for (int i = 0; i < s.size(); i++) {
		if (s[i] == ' ') {
			if (tmp == t) {
				ans++;
				loc = min(loc, i - m);
			}
			tmp = "";
		} else tmp += s[i];
	}
	if (!ans) return puts("-1"), 0;
	cout << ans << ' ' << loc;
	return 0;
}
