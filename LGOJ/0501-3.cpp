#include <bits/stdc++.h>
using namespace std;
int n;
int main() {
	cin >> n;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (abs(i - j) == n / 2 || i + j == n / 2 || i + j == 3 * n / 2 - 1) putchar('#');
			else putchar('.');
		}
		puts("");
	}
	return 0;
}
