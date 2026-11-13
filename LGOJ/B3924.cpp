#include <bits/stdc++.h>
using namespace std;
int n;
char a[50][50];
int main() {
	cin >> n;
	memset(a, 97, sizeof(a));
	for (int i = 1; i <= n; i++) a[i][1] = a[i][n] = '|';
	for (int j = 2; j < n; j++) a[(n + 1) / 2][j] = '-';
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) cout << a[i][j];
		cout << endl;
	}
	return 0;
}
