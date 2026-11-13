#include <bits/stdc++.h>
using namespace std;
int t, ans;
string s;
int main() {
	cin >> t >> s;
	while (s.find("not") != -1) {
		ans++;
		if (s == "not") {
			s = "";
			break;
		}
		for (int i = 0; i < s.size() - 2; i++) {
			if (s.substr(i, 3) != "not") continue;
			s.erase(i, 3);
			i--;
		}
	}
	cout << s << endl << ans;
	return 0;
}
/*
not

012345
nnotot
*/
