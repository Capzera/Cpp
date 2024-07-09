#include <bits/stdc++.h>
using namespace std;
int n, k, a[200010];
int main() {
	cin >> n >> k;
	for (int i = 1; i <= n; i++) cin >> a[i];
	cout << a[(n + k) / 2];
	return 0;
}
