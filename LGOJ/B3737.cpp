#include <bits/stdc++.h>
using namespace std;
int x, y, day, ans;
int main() {
	day = 5;
	cin >> x >> y;
	for (int i = 1900; i <= y; i++) {
		day++;
		day += i % 400 == 0 || i % 4 == 0 && i % 100 != 0;
		day %= 7;
		ans += i >= x && day > 4;
	}
	cout << ans;
	return 0;
}
