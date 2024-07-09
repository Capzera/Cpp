#include <bits/stdc++.h>
using namespace std;
int a[200010];
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	int n, k;
	cin >> n >> k;
	int m = (n - k) / 2;
	for (int i = 1; i <= n; i++) cin >> a[i];
	cout << a[k + m];
	return 0;
}
