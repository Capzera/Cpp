#include <bits/stdc++.h>
using namespace std;
string s, ans;
map<string, int> mp;
int n, mx;
int main() {
	cin >> n;
	while (n--) {
		cin >> s;
		for (auto& ch : s) ch = tolower(ch);
		if (++mp[s] > mx) {
			mx = mp[s];
			ans = s;
		}
	}
	cout << ans;
	return 0;
}
