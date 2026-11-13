#include <bits/stdc++.h>
using namespace std;
int t, n, m;
string s;
char ch;
char inv(char x) {
	return 97 - x;
}
string solve() {
	cin >> n >> m >> s;
	if (n == 1 || n == m || s == "01" || s == "10") return "-1";
	string ans = s, t = s.substr(1);
	sort(t.begin(), t.end());
	if (t[0] == t.back() && t[0] != s[0]) {
		ans.insert(n - 1, 1, inv(s.back()));
		while (--m > n) {
			ans.insert(0, 1, s[0]);
		}
		return ans;
	}
	while (m-- > n) {
		ans.insert(1, 1, inv(s[0]));
	}
	return ans;
}

int main() {
	cin >> t;
	while (t--) cout << solve() << '\n';
	return 0;
}



