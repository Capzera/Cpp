#include <bits/stdc++.h>
using namespace std;
long long n, k, a[2001];
int main() {
	cin >> n >> k;
	for (int i = 1; i <= n; i++) cin >> a[i];
	int i = 1, j = k + 1;
	while (i <= k && j <= n) cout << a[i++] << " " << a[j++] << " ";
	while (i <= k) cout << a[i++] << " ";
	while (j <= n) cout << a[j++] << " ";
	return 0;
}
