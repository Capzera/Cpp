#include <bits/stdc++.h>
using namespace std;
int n, c1, c2, x;
int main() {
	cin >> n;
	for (int i = 1 ; i <= n; i++) {
		cin >> x;
		if (x == 1) break;
		if (!x) c2++;
		c1++;
	}
	cout << c1 << " " << c2 << '\n';
	return 0;
}
