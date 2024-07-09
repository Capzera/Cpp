#include <bits/stdc++.h>
using namespace std;
int a[200010];
int main() {
	int n, k;
	cin >> n >> k;
	int m = (n - k) / 2;
	for (int i = 1; i <= n; i++) cin >> a[i];
	cout << a[k + m];
	return 0;
}
