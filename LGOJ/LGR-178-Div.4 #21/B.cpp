#include <iostream>
using namespace std;
int main() {
	long long x, y, diff;
	cin >> x >> y;
	diff = y - x;
	if (diff <= 24000) {
		cout << 5;
	} else if (diff <= 36000) {
		cout << 6;
	} else if (diff <= 48000) {
		cout << 8;
	} else if (diff <= 72000) {
		cout << 11;
	} else {
		long long bonus = diff % 96000, ans = diff / 96000 * 13;
		if (bonus) ans += 13;
		cout << ans;
	}
	return 0;
}
