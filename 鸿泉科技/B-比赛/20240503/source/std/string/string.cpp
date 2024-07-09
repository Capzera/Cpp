#include <bits/stdc++.h>
using namespace std;
int main() {
	freopen("string.in", "r", stdin);
	freopen("string.out", "w", stdout);
	string s;
	cin >> s;
	int n = s.size();
	long long ans = 0;
	for (int i = 0, j = 0; i < n; i++) {
		if (i && s[i] == s[i - 1]) j = i;
		ans += i - j + 1;
	}
	cout << ans << endl; 
	return 0;
}
