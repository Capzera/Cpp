#include <bits/stdc++.h>
using namespace std;
int x, y, ans;
int main() {
	cin >> x >> y;
	for (int i = x + 1; i < y; i++) {
		ans += i * (i % 400 == 0 || i % 4 == 0 && i % 100 != 0);
	}
	cout << ans;
	return 0;
}
