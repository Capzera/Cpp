#include <bits/stdc++.h>
using namespace std;
long long n, mx, a[101];
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
		if (a[i] > a[mx]) mx = i;
	}
	if (a[1] > a[n]) swap(a[1], a[n]);
	else if (a[n] != a[mx]) swap(a[mx], a[n]);
	for (int i = 1; i <= n; i++) cout << a[i] << " ";
 	return 0;
}
