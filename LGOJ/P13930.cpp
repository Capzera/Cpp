#include <bits/stdc++.h>
using namespace std;
string s;
int ans;
map<char, int> mp;
int main() {
	cin >> s;
	for (auto& ch : s) mp[ch]++;
	for (auto&& it = mp.begin(); it != mp.end(); it++) {
		ans = max(ans, it->second);
	}
	for (auto&& it = mp.begin(); it != mp.end(); it++) {
		if (ans == it->second) cout << it->first;
	}
	return 0;
}
