#include <bits/stdc++.h>
using namespace std;
int t, num, f = 1;
string s;
void solve() {
	cin >> s;
	s += 'a';
	int n = s.size(), ans = 0;
	for (int i = 0; i < n; i++) {
		if (isdigit(s[i])) {
			num = num * 10 + s[i] - '0';
			continue;
		}
		ans += num * f, num = 0, f = 1;
		if (s[i] == '-') f = -1;
		if (isupper(s[i])) cout << s[i];
	}
	cout << ans << endl;
}
int main() { 
	cin >> t;
	while (t--) solve();
	return 0;
}
