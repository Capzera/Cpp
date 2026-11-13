#include <bits/stdc++.h>
using namespace std;
int n, x, a[10010];
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
		if (a[i] <= 15) cout << a[i] << " ";
	}
	for (int i = 1; i <= n; i++) {
		if (a[i] > 15) cout << a[i] << " ";
	}
	return 0;
}
