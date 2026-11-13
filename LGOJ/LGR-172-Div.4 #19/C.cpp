#include <bits/stdc++.h>
using namespace std;
long long n, cnt = 1, a[1000010];
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = 1; i <= n; i += a[i], cnt++) {
		if (i == n) break;
		if (i + a[i] > n) {
			cout << "No\n" << cnt;
			return 0;
		}
	}
	cout << "Yes\n" << cnt - 1;
	return 0;
}
