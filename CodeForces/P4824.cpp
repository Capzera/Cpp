#include <bits/stdc++.h>
using namespace std;
const int N = 1e6 + 10;
string s, p;
int n, m, f[N];
int top, stk[N];
int main() {
	cin >> s >> p;
	n = s.size(), m = p.size();
	vector<int> next(m + 1);
	int i = 0, j = next[0] = -1;
	while (i < m) {
		if (j == -1 || p[i] == p[j]) {
			i++, j++;
			next[i] = j;
		} else j = next[j];
	}
	for (int i = 0, j = 0; i < n; i++) {
		stk[++top] = s[i];
		while (j != -1 && s[i] != p[j]) j = next[j];
		j++;
		f[top] = j;
		if (j == m) {
			top -= m;
			j = f[top];
		}
	}
	for (int i = 1; i <= top; i++) putchar(stk[i]);
	return 0;
}
