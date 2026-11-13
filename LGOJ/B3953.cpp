#include <bits/stdc++.h>
using namespace std;
int n, d[32]; // 存储n的因数
int main() {
	cin >> n;
	int i = 1, j = 0;
	for (; i * i < n; i++) {
		if (n % i == 0) {
			d[j++] = i;
			cout << i << endl;
		}
	}
	if (i * i == n) {
		cout << i << endl;
	}
	for (j = j - 1; j >= 0; j--) {
		cout << n / d[j] << endl;
	}
	return 0;
}
/*
1000000000
63246
20
1 2 4 5 10 20
144
1 2 3 4 6 8 9 12 16 18 24 36 48 72 144
d[0] = 1, j = 1;
d[1] = 2, j = 2l
d[2] = 3, j = 3;
d[3] = 4, j= 4;
d[4] = 6, j = 5;
d[5] = 8, j = 6;
d[6] = 9, j = 7;
3 x 10^8
n <= 10^9
1 - sqrt(n)
2 * sqrt(n);
*/
