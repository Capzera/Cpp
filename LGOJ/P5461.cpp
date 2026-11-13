#include <bits/stdc++.h>
using namespace std;
int n, a[1025][1025];
void fun(int x, int y, int n) {
	if (n == 2) {
		a[x + 1][y] = a[x][y + 1] = a[x + 1][y + 1] = 1;
		return;
	}
	fun(x + n / 2, y, n >> 1);
	fun(x, y + n / 2, n >> 1);
	fun(x + n / 2, y + n / 2, n >> 1);
}
int main() {
	cin >> n;
	n = 1 << n;
	fun(0, 0, n);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) cout << a[i][j] << " ";
		cout << endl;
	}
	return 0;
}
