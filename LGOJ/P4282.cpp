#include <bits/stdc++.h>
using namespace std;
const int N = 100001;
int a[N], b[N], r[N], ans[N], n, x;
char op;
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> r[i];
	for (int i = 1; i <= n; i++) cin >> a[i];
	cin >> op;
	for (int i = 1; i <= n; i++) cin >> b[i];
	for (int i = n; i >= 1; i--) {
		x = a[i] + (op == '+' ? x + b[i] : r[i] - b[i] - x);
		ans[i] = x % r[i];
		x = x / r[i] ^ (op != '+');
	}
	for (int i = 1; i <= n; i++) cout << ans[i] << ' ';
	return 0;
}
