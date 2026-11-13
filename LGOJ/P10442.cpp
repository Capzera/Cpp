#include <bits/stdc++.h>
using namespace std;
string s, t;
map<char, int> mp;
int ans;
int main() {
	cin >> s >> t;
	for (auto& x : t) mp[x]++;
	for (auto& x : s) {
		if (mp[x]) {
			ans++;
			mp[x]--;
		}
	}
	cout << ans;
	return 0;
}
