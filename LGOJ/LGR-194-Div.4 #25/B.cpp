#include <bits/stdc++.h>
using namespace std;
long long x, y, z, w;
int main() {
	cin >> x >> y >> z >> w;
	for (int i = 1; i <= 1001; i++) {
		if (i * z == x && i * w == y) {
			cout << i;
			return 0;
		}
	}
	cout << -1;
	return 0;
}
