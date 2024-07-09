#include <bits/stdc++.h>
using namespace std;
int main() {
	int n;
	cin >> n;
	int a = 0, b = 0;
	for (int i = 1; i <= n; i++) {
		if (i % 2 == 1) a += i;
		else b += i;
	}
	cout << a << " " << b;
	return 0;
}
