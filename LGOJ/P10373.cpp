#include <bits/stdc++.h>
using namespace std;
long long q, x, rt;
int main() {
	cin >> q;
	while (q--) {
		scanf("%lld", &x);
		rt = cbrt(x + 0.5);
		cout << (-1ll * rt * rt * rt * rt - 2 * rt * rt * rt - rt * rt + 4 * rt * (x + 1)) / 4 << '\n';
	}
	return 0;
}
