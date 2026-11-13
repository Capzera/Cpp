#include <bits/stdc++.h>
using namespace std;
long long n, m, a[200010], sum;
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) cin >> a[i];
	sort(a + 1, a + n + 1, greater<long long>());
	for (int i = 1; i <= n; i++) {
		sum += a[i];
		if (sum > m) {
			cout << i;
			return 0;
		}
	}
	return 0;
}
