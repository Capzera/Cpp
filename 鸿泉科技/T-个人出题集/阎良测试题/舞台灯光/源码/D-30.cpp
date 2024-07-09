#include <bits/stdc++.h>
using namespace std;
int main() {
	int a = 1, b = 2, c = 2, n;
	cin >> n;
	for (int i = 2; i <= n; i++) {
		c = a + b;
		a = b;
		b = c;
	}
	cout << c << endl;
	return 0;
}
