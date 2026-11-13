#include <bits/stdc++.h>
using namespace std;
long long n, q, h[1001], l, r, k;
string op;
int main() {
	cin >> n >> q;
	while (q--) {
		cin >> op >> l >> r;
		if (op == "water") {
			for (int i = l; i <= r; i++) h[i]++;
		} else {
			int ans = 0;
			cin >> k;
			for (int i = l; i <= r; i++) {
				if (h[i] >= k) {
					h[i] = 0;
					ans++;
				}
			}
			cout << ans << endl;
		}
	}
	return 0;
}
