#include <bits/stdc++.h>
#define int long long
using namespace std;
string s, s1 = "boy", s2 = "girl";
long long i, l, r, ans1, ans2;
signed main() {
	cin >> s;
	int n = s.size();
	for (int i = 0; i < n; i++) {
		if (s[i] == '.') continue;
		if (i + 3 < n && s.substr(i, 4) == s2) {
			ans2++, i += 3;
			continue;
		}
		if (i + 2 < n) {
			if (s.substr(i, 3) == s1) {
				ans1++, i += 2;
				continue;
			}
			if (s.substr(i, 3) == "gir" || s.substr(i, 3) == "irl") {
				ans2++, i += 2;
				continue;
			}
		}
		if (i + 1 < n) {
			string t = s.substr(i, 2);
			if (t == "bo" || t == "oy") {
				ans1++, i++;
				continue;
			}
			if (t == "gi" || t == "ir" || t == "rl") {
				ans2++, i++;
				continue;
			}
		}
		if (s1.find(s[i]) != -1) ans1++;
		else ans2++;
	}
	cout << ans1 << endl << ans2;
	return 0;
}
/*
boyoyir
2 1
boyobir
3 1
*/
