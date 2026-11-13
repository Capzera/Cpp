#include <bits/stdc++.h>
using namespace std;
int n, a[41][41];
int main() {
	cin >> n;
	for (int i = 1, x = 0, y = n >> 1; i <= n * n; i++) {
		a[x][y] = i;
		int mx = (x - 1 + n) % n, my = (y + 1) % n;
		if (a[mx][my]) x++;
		else x = mx, y = my;
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) cout << a[i][j] << " ";
		cout << endl;
	}
	return 0;
}
