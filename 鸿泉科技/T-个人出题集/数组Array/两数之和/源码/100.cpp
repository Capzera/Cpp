#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, flag = 1;
	cin >> n;
	int ans = 0;
	vector<int> a(n);
	for (auto& x : a) cin >> x;
	for (int k = 0; k < n; k++) {
		flag = 0;
		for (int i = 0; i < n - 1; i++) {
			if (flag) break;
			for (int j = i + 1; j < n; j++) {
				if (k == i || k == j) continue;
				if (a[i] + a[j] == a[k]) {
					flag = 1;
					break;
				}
			}
		}
		if (flag == 1) ans++;
	}
	cout << ans;
	return 0;
}
