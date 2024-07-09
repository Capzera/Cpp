#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, m, x, l, r;
	cin >> n >> m;
	vector<int> sum(n + 1);
	for (int i = 1; i <=n; i++) {
		cin >> x;
		sum[i] = sum[i - 1] + x;
	}
	while (m--) {
		cin >> l >> r;
		if (l > r) swap(l, r);
		cout << sum[r] - sum[l - 1] << endl;
	}
	return 0;
}
