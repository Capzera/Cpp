#include <bits/stdc++.h>
using namespace std;
int n, m, a[1000010], x;
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) cin >> a[i];
	while (m--) {
		cin >> x;
		int loc = lower_bound(a + 1, a + n + 1, x) - a;
		if (a[loc] != x) loc = -1;
		cout << loc << " ";
	}
	return 0;
}
