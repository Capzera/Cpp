#include <bits/stdc++.h>
using namespace std;
string s, p;
int pi[1000100];
int main() {
	cin >> s >> p;
	int n = s.size(), m = p.size();
	s = "#" + s, p = "#" + p;
	for (int i = 2, j = 0; i <= m; i++) {
		while (j && p[i] != p[j + 1]) j = pi[j];
		if (p[i] == p[j + 1]) j++;
		pi[i] = j;
	}
	for (int i = 1, j = 0; i <= n; i++) {
		while (j && s[i] != p[j + 1]) j = pi[j];
		if (s[i] == p[j + 1]) j++;
		if (j == m) {
			cout << i - j + 1 << '\n';
			j = pi[j];
		}
	}
	for (int i = 1; i <= m; i++) cout << pi[i] << " ";
	return 0;
}
