#include <bits/stdc++.h>
using namespace std;
const int N = 1e6 + 10;
int n, ans, a[N], st[N], ed[N];
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
		ed[i] = a[i] > a[i - 1] ? ed[i - 1] + 1 : 1;
	}
	for (int i = n; i > 0; i--) {
		st[i] = a[i] < a[i + 1] ? st[i + 1] + 1 : 1;
	}
	for (int i = 1; i <= n; i++) {
		if (a[i - 1] < a[i + 1] - 1) ans = max(ans, ed[i - 1] + st[i + 1] + 1);
		if (a[i - 1] >= a[i]) ans = max(ans, ed[i - 1] + 1);
		if (a[i] >= a[i + 1]) ans = max(ans, st[i + 1] + 1);
	}
	cout << ans;
	return 0;
}
