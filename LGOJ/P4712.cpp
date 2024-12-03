#include <bits/stdc++.h>
long long a[100005], b, n, last, j;
long long pre;
using namespace std;
int main() {
	cin >> n >> pre;
	for (int i = 1; i <= n; i++) {
		cin >> a[i] >> b;
		for (j = last + 1; j <= b; j++) pre += a[j];
		last = j - 1;
		pre -= a[i] * 5;
		if (pre < 0) {
			cout << -1;
			return 0;
		}
	}
	for (int i = last + 1; i <= n; i++) pre += a[i];
	double ans = pre / 5.0;
	cout << ans << endl;
	return 0;
}
