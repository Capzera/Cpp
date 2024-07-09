#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, x, y;
	cin >> n >> x >> y;
	vector<int> f(n + 1);
	if (1 != x) f[1] = 1;
	if (x != 2 && y != 2) f[2] = 1;
	for (int i = 3; i <= n; i++) {
		if (x == i || y == i) continue;
		f[i] = f[i - 1] + f[i - 2];
	}
	cout << f[n] << endl;
	return 0;
}
