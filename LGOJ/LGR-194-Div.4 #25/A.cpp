#include <bits/stdc++.h>
#define int long long
using namespace std;

signed main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	long long a, b, c;
	cin >> a >> b;
	c = 10000 - (a / 10) - (b * 10);
	cout << a / 10 << " " << b * 10 << " " << c;
	return 0;
}
