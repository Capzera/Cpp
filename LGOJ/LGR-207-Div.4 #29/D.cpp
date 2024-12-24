#include <bits/stdc++.h>
using namespace std;
int n, a[1001], ans, mx;
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = n; i > 0; i--) {
		if (a[i] > mx) {
			mx = a[i];
			ans++;
		}
	}
	cout << ans;
	return 0;
}
/*
求有多少数大于它 后面的 最大值
a[i] > 后面的最大值 ans++;
a[i]
*/
