#include <bits/stdc++.h>
using namespace std;
int x, y, ans;
int main() {
	cin >> x >> y;
	for (int a = x; a <= y; a++) {
		for (int b = x; b <= y; b++) {
			for (int c = x; c <= y; c++) {
				for (int d = x; d <= y; d++) {
					if (a * d == b * c) ans++;
				}
			}
		}
	}
	cout << ans;
	return 0;
}
