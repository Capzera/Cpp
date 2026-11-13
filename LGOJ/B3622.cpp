#include <bits/stdc++.h>
using namespace std;
int n;
int main() {
	cin >> n;
	for (int i = 0; i < 1 << n; i++) {
		for (int j = 0; j < n; j++) {
			cout << (i >> (n - j - 1) & 1 ? 'Y' : 'N');
		}
		puts("");
	}
	return 0;
}
