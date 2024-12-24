#include <bits/stdc++.h>
using namespace std;
long long n, m, x, st, ed, l, ans;
int main() {
	cin >> n >> m >> st >> ed;
	while (m--) {
		cin >> l;
		int f = 0, ret = 0;
		for (int i = 1; i <= l; i++) {
			cin >> x;
			if (x == st) f = 1;
			if (f && x == ed) ret = 1;
		}
		ans += ret;
	}
	cout << ans;
	return 0;
}
