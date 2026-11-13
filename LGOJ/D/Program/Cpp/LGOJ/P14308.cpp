#include <bits/stdc++.h>
using namespace std;
long long T, x, y, f[93];
struct cube {
	int lu, ld, ru, rd;
} mat[93];
long long solve() {
	cin >> x >> y;
	for (int i = 1; i <= 92; i++) {
		
	}
}
int main() {
	cin >> T;
	f[1] = f[2] = 1;
	for (int i = 3; i <= 92; i++) f[i] = f[i - 1] + f[i - 2];
	mat[1] = {};
	while (T--) cout << solve() << '\n';
	return 0;
}
ad     ac    bc   bd
-1 1  -1 0   0 0  0 1      -1 0 0 1      1   1     -1   0
-1 0  -1 -1  0 -1  0 0     -1 0 -1 0     2   1     -1   -1
0 1   0 -1   2 -1  2  1     0 2 -1 1     3   2      0   -1
-1 4  -1  1  2  1  2  4    -1 2  1  4    4   3     -1   1
-6 4  -6 -1  -1 -1 -1 4    -6 -1 -1 4    5   5     -6   -1
-6 -1 -6 -9  2  -1 2 -9    -6 2  -1 -9   6   8     -6   -1

