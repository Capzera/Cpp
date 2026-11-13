#include <bits/stdc++.h>
using namespace std;
long long n, m, x, y, a[100010], now;
string name[100010];
int main() {
	cin >> n >> m;
	for (int i = 0; i < n; i++) {
		cin >> a[i] >> name[i];
	}
	while (m--) {
		cin >> x >> y;
		int k = x ^ a[now] ? 1 : -1;
		now = (now + k * y + n) % n;
	}
	cout << name[now];
	return 0;
}
