#include <bits/stdc++.h>
using namespace std;
const int N = 1e6 + 10;
string s, t;
int n, pi[N];
int kmp(string s, string t) {
	s = " " + s + "#" + t;
	int n = s.size(), m = t.size(), ans = 0;
	for (int i = 2, j = 0; i < n; i++) {
		while (j && s[i] != s[j + 1]) j = pi[j];
		if (s[i] == s[j + 1]) j++;
		pi[i] = j;
		if (i >= m && i + pi[i] < n) ans = max(ans, pi[i]);
	}
	return ans;
}
int main() {
	cin >> s;
	n = s.size();
	int l = 0, r = n - 1;
	while (l < r && s[l] == s[r]) l++, r--;
	t = s = s.substr(l, n - 2 * l);
	reverse(t.begin(), t.end());
	cout << l + max(kmp(s, t), kmp(t, s));
	return 0;
}
