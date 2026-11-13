#include <bits/stdc++.h>
using namespace std;
string s;
int pi[800010];
int main() {
	cin >> s;
	int n = s.size();
	string t = s;
	reverse(t.begin(), t.end());
	string p = " " + s + "#" + t;
	for (int i = 2, j = 0; i <= n * 2 + 1; i++) {
		while (j && p[i] != p[j + 1]) j = pi[j];
		if (p[i] == p[j + 1]) j++;
		pi[i] = j;
	}
	cout << t.substr(0, n - pi[n << 1 | 1]) + s;
	return 0;
}


