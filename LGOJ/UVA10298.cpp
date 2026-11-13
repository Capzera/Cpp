#include <bits/stdc++.h>
using namespace std;
string s;
int t, n, pi[1000010];
int solve() {
	n = s.size();
	s = "#" + s;
	for (int i = 2, j = 0; i <= n; i++) {
		while (j && s[i] != s[j + 1]) j = pi[j];
		if (s[i] == s[j + 1]) j++;
		pi[i] = j;
	}
	int ans = n - pi[n];
	return (n + ans - 1) / ans;
}

int main() {
	while (cin >> s, s != ".") {
		cout << solve() << '\n';
	}
	return 0;
}

