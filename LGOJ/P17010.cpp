#include <bits/stdc++.h>
using namespace std;
long long n, ans, sum, a[1001];
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i];
	sort(a + 1, a + n + 1, greater<long long>());
	for (int i = 1; i <= n; i++) {
		sum += a[i];
		ans += sum;
	}
	cout << ans;
	return 0;
}
