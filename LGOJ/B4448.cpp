#include <bits/stdc++.h>
using namespace std;
long long h, w, x, ans;
int main() {
	cin >> h >> w >> x;
	for (int r = 1; r <= h; r++) {
		for (int c = 1; c <= w; c++) {
			long long t = x + r - c;
			if (r * r + c * c <= t * t) ans++;
		}
	}
	cout << ans;
 	return 0;
}
