#include <bits/stdc++.h>
using namespace std;
string s, t, p;
long long ans;
int main() {
	cin >> s;
	int n = s.size();
	for (int i = 5; i < n; i++) {
		if (s[i] != 'd' || s.substr(i - 5, 6) != "friend") continue;
		ans++;
		s[i] = 'l';
		if (i + 1 < n) s[i + 1] = 'o';
		if (i + 2 < n) s[i + 2] = 'v';
		if (i + 3 < n) s[i + 3] = 'e';
		i += 5;
	}
	cout << ans;
	return 0;
}
