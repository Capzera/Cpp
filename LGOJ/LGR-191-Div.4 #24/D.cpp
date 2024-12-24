#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, x;
	cin >> n;
	while (n--) {
		cin >> x; // 取a1 + b1 = a2 + b2 + ... + an + bn = S 当S = 0 bi = -ai
		cout << -x << " ";
	}
	return 0;
}
