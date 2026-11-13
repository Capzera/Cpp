#include <bits/stdc++.h>
using namespace std;
const int K = 30;
long long n, ans, a[250001], base[K];
int main() {
	base[0] = 1;
	for (int i = 1; i < K; i++) base[i] = base[i - 1] << 1;
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = 2; i <= n; i++) {
		for (int j = 0; j < K; j++) {
			if (a[i] * base[j] >= a[i - 1]) {
				ans += j;
				a[i] *= base[j];
				break;
			}
		}
	}
	cout << ans;
	return 0;
}
