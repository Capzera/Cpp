#include <bits/stdc++.h>
using namespace std;
int n, m;
int main() {
	cin >> n >> m;
	for (int i = 1; i <= 2 * n + 1; i++) {
		for (int j = 1; j <= 2 * m + 1; j++) {
			i & 1 ? putchar(j & 1 ? '+' : '-') : putchar(j & 1 ? '|' : ' ');
		}
		puts("");
	}
	return 0;
}
