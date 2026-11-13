#include <bits/stdc++.h>
using namespace std;
string s;
int main() {
	cin >> s;
	int n = s.size(), l = 0, r = n - 1, ans = n;
	while (l < r) {
		while (l < r && s[l] != 'A') l++;
		while (l < r && s[r] != 'B') r--;
		if (l >= r) break;
		ans -= 2;
		l++, r--;
	}
	cout << ans;
	return 0;
}

