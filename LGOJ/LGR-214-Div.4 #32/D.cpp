#include <bits/stdc++.h>
using namespace std;
long long x, y, z, ans, cnt;
int main() {
	cin >> x >> y >> z;
	for (int i = 0; i <= x; i++) {
		for (int j = 1; j <= y; j++) {
			for (int k = 0; k <= z; k++) {
				int deli = (i + j) ^ k;
				if (deli > ans) {
					ans = deli;
					cnt = 0;
				} 
				if (deli == ans) cnt++;
			}
		}
	}
	cout << ans << endl << cnt;
	return 0;
}
