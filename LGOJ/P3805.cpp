#include <bits/stdc++.h>
using namespace std;
int p[22000001], ans;
char ch;
int main() {
	string s = "##";
	while (cin >> ch) s += ch, s += "#";
	for (int l = 0, j = 0, r = 0; l < s.size(); l++) {
		p[l] = r > l ? min(p[j * 2 - l], r - l) : 1;
		while (s[l + p[l]] == s[l - p[l]]) p[l]++;
		if (l + p[l] > r) {
			r = l + p[l];
			j = l;
		}
		ans = max(ans, p[l] - 1);
	}
	cout << ans;
	return 0;
}

