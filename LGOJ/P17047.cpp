#include <bits/stdc++.h>
using namespace std;
long long n, cnt, f, ans = 0;
string s, op;
unordered_map<string, int[3]> mp;
int main() {
	cin >> n;
	while (n--) {
		cin >> s >> op >> cnt;
		if (!mp.count(s)) mp[s][0] = mp[s][1] = mp[s][2] = 0;
		int x = op == "left" ? 0 : op == "right" ? 1 : 2;
		mp[s][x] += cnt;
	}
	for (auto& [s, vec] : mp) {
		int lt = vec[0], rt = vec[1], an = vec[2];
		ans += max({lt, rt, min(an, 1)});
		if (lt && rt || an > 1 || an == 1 && (lt | rt)) f = 1;
	}
	if (!f) return puts("impossible"), 0;
	cout << ans + 1;
	return 0;
}
