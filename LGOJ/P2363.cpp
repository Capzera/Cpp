#include <bits/stdc++.h>
using namespace std;
long long n, a[51][51], ans;
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			cin >> a[i][j];
			a[i][j] += a[i - 1][j] + a[i][j - 1] - a[i - 1][j - 1];
		}
	}
	for (int x1 = 1; x1 <= n; x1++) {
		for (int y1 = 1; y1 <= n; y1++) {
			for (int x2 = x1; x2 <= n; x2++) {
				for (int y2 = y1; y2 <= n; y2++) {
					long long sum1 = a[x2][y2] - a[x2][y1 - 1] - a[x1 - 1][y2] + a[x1 - 1][y1 - 1];
					for (int x3 = x2 + 1; x3 <= n; x3++) {
						for (int y3 = y2 + 1; y3 <= n; y3++) {
							long long sum2 = a[x3][y3] - a[x2][y3] - a[x3][y2] + a[x2][y2];
							if (sum1 == sum2) {
								ans++;
							}
						}
					}
					for (int x3 = x1 - 1; x3 > 0; x3--) {
						for (int y3 = y2 + 1; y3 <= n; y3++) {
							long long sum2 = a[x1 - 1][y3] - a[x1 - 1][y2] - a[x3 - 1][y3] + a[x3 - 1][y2];
							if (sum1 == sum2) {
								ans++;
							}
						}
					}
				}
			}
		}
	}
	cout << ans;
	return 0;
}
/*
1 2 3
4 5 6
7 8 9
0  0  0  0
0  1  3  6
0  5  12 21
0  12 27 45
x1 y1 = 3, 1  x2 y2 = 3, 1
x1, y2 = 3, 1
x3, y3 = 1, 2
2 2 - 2 1 - 1, 2 + 1, 1
(2, 2) - (1, 2) = 12 - 5 - 0 + 0 = 7
= (2, 2) - (2, 1) - (0, 2) + (0, 1) = 
x3, y3 = 1, 2    
12 - 0 - 5 
sum1 = 7
sum2 = 27 - 
2, 2



*/
