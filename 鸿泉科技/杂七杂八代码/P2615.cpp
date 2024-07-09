#include <bits/stdc++.h>
using namespace std;
int a[40][40];
int main() {
	int n;
	cin >> n;
	int x = 0, y = n / 2;
	for (int i = 1; i <= n * n; i++) {
		a[x][y] = i;
		x = (x - 1 + n) % n, y = (y + 1) % n;
		if (a[x][y] != 0) {
			x = (x + 2) % n, y = (y - 1 + n) % n;
		}
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) cout << a[i][j] << " ";
		cout << endl;
	}
	return 0;
}

