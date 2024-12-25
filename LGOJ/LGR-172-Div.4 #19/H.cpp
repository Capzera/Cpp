#include <bits/stdc++.h>
using namespace std;
string s, t;
int n, ans;
int main() {
	cin >> s;
	n = s.size();
	for (int i = 0; i < n - 6; i++) {
		if (s.substr(i, 7) != "Genshin") continue;
		for (int j = i + 1; j < n - 5; j++) {
			ans += s.substr(j, 6) == "player";
		}
	}
	cout << ans;
	return 0;
}
