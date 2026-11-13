#include <bits/stdc++.h>
using namespace std;
int n, a[50][50];
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		a[i][1] = 1;
		for (int j = 2; j <= i; j++) {
			a[i][j] = a[i - 1][j] + a[i - 1][j - 1];
		}
	}
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= i; j++) {
			cout << a[i][j] << " ";
		}
		cout << '\n';
	}
	return 0;
}
