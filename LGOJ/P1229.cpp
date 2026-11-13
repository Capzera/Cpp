#include <bits/stdc++.h>
using namespace std;
string pre, pos;
unordered_map<char, int> mp;
long long ans;
int main() {
	cin >> pre >> pos;
	for (int i = 0; i < pos.size(); i++) mp[pos[i]] = i;
	for (int i = 0; i < pre.size(); i++) {
		int j = mp[pre[i]];
		if (!j) continue;
		if (pre[i + 1] == pos[j - 1]) ans++;
	}
	cout << (1ll << ans);
	return 0;
}
