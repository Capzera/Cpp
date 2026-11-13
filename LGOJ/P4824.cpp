#include <bits/stdc++.h>
using namespace std;
const int N = 1e6 + 10;
string s, p;
int n, m, f[N], pi[N];
int top, stk[N];
int main() {
	cin >> s >> p;
	n = s.size(), m = p.size();
	s = "#" + s, p = "#" + p;
	for (int i = 2, j = 0; i <= m; i++) {
		while (j && p[i] != p[j + 1]) j = pi[j];
		if (p[i] == p[j + 1]) j++;
		pi[i] = j;
	}
	for (int i = 1, j = 0; i <= n; i++) {
		stk[++top] = s[i];
		while (j && s[i] != p[j + 1]) j = pi[j];
		if (s[i] == p[j + 1]) j++;
		f[top] = j;
		if (j == m) {
			top -= m;
			j = f[top];
		}
	}
	for (int i = 1; i <= top; i++) putchar(stk[i]);
	return 0;
}
