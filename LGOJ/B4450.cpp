#include <bits/stdc++.h>
using namespace std;
long long n, m, x, y, sum, a[100001];
int main() {
	cin >> n >> m;
	memset(a, 0x3f, sizeof(a));
	while (m--) {
		cin >> x >> y;
		a[x] = min(a[x], y);
	}
	for (int i = 1; i <= n; i++) sum += a[i];
	cout << sum;
	return 0;
}
