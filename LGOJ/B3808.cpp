#include <bits/stdc++.h>
using namespace std;
long long n, k, a[1000010], sum[10001];
int main() {
	cin >> n >> k;
	for (int i = 1; i <= 2 * n; i++) {
		cin >> a[i];
		sum[i % k] += a[i];
	}
	for (int i = 1; i <= 2 * n; i++) {
		if (i & 1) a[i] = sum[i % k] % i;
		cout << a[i] << " ";
	}
	return 0;
}
