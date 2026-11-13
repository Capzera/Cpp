#include <bits/stdc++.h>
using namespace std;
string s;
int t;
void solve() {
	cin >> s; s += 'a';
	int n = s.size(), ans = 0, num = 0, f = 1;
	for (int i = 0; i < n; i++) {
		
		
		if (isdigit(s[i])) num = num * 10 + s[i] - '0';
		else if (i) {
			if (isdigit(s[i - 1])) ans += f * num;
			f = 1, num = 0;
		}
		if (isupper(s[i])) cout << s[i];
		if (s[i] == '-') f = -1;
		cout << ans << endl;
	}
	cout << ans << endl;
}
int main() {
	cin >> t;
	while (t--) solve();
	return 0;
}
/*

1
393a5a6a-9-8
*/
