#include <bits/stdc++.h>
using namespace std;
int n;
map<string, string> mp;
string s, t, ans;
int main() {
	cin >> n;
	while (n--) {
		cin >> s >> t;
		mp[s] = t;
	}
	cin >> s;
	s += " ", t = "";
	for (auto& x : s) {
		if (!isalpha(x)) {
			if (t == "" || mp.count(t)) ans += mp[t];
			else ans += "UNK";
			t = "";
			ans += x;
		} else t += x;
	}
	cout << ans;
	return 0;
}
