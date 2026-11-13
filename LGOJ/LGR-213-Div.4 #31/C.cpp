#include <bits/stdc++.h>
using namespace std;
int x, c, k, ans;
int main() {
	cin >> x >> c >> k;
	while (--k) {
		x = (x * x + c) % 10000;
	}
	cout << x;
	return 0;
}
