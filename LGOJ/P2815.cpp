#include <bits/stdc++.h>
using namespace std;
string s, ans, tmp;
int mx, cnt, loc;
int main() {
	cin >> s;
	for (int i = 0; i < s.size(); i += 5) {
		string mod = s.substr(i, 4), nxt = "0";
		for (int j = 0; j < 4; j++) {
			if (mod[j] == '0') continue;
			nxt = mod.substr(j);
			break;
		}
		ans += nxt + ":";
	}
	for (int i = 0; i < ans.size(); i++) {
		if (ans[i] == ':') {
			if (tmp == "0") {
				cnt++;
			} else {
				if (cnt > mx) {
					mx = cnt;
					loc = i;
				}
				cnt = 0;
			}
			tmp = "";
		} else tmp += ans[i];
	}
	if (cnt > mx) {
		mx = cnt;
		loc = ans.size() - 1;
	}
	if (mx) ans.replace(max(loc - 2 * mx + 1, 0), 2 * mx, "::");
	if (ans == ":") return puts("::"), 0;
	if (ans.back() == ':' && ans[ans.size() - 2] != ':') ans.pop_back();
	cout << ans;
	return 0;
}
