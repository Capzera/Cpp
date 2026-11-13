#include <bits/stdc++.h>
using namespace std;
int n, x, a[200010], mp[11];
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
	}
	for (int i = 0; i <= 9; i++) cin >> mp[i];
	for (int i = 1; i <= n; i++) cout << mp[a[i]] << " ";
	return 0;
}
