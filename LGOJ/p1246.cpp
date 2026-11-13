#include <bits/stdc++.h>
using namespace std;
string s;
long long n, ans, cnt, f[27][7];
int main() {
	cin >> s;
	n = s.size();
	for (int i = 1; i < n; i++) {
		if (s[i] <= s[i - 1]) return puts("0"), 0;
	}
	for (int i = 1; i <= 26; i++) f[i][1] = 1;
	for (int j = 2; j <= 6; j++)
		for (int i = 27 - j; i > 0; i--)
			f[i][j] = f[i + 1][j - 1] + f[i + 1][j];
	for (int j = n - 1; ~j; j--) {
		cnt++;
		for (int i = 1; i <= s[j] - 'a' + 1; i++) ans += f[i][cnt];
	}
	cout << ans;
	return 0;
}
