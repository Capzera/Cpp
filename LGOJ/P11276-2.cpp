#include <bits/stdc++.h>
using namespace std;
const int N = 1e6 + 10;
string p;
int n, pi[N];
int main() {
	cin >> p;
	n = p.size();
	p = "#" + p;
	for (int i = 2, j = 0; i <= n; i++) {
		while (j && p[i] != p[j + 1]) j = pi[j];
		if (p[i] == p[j + 1]) j++;
		pi[i] = j;
	}
	cout << p.substr(1, n - pi[n]) + p.substr(1);
	return 0;
}
