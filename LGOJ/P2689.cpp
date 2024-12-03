#include <iostream>
using namespace std;
long long r1, c1, r2, c2, m, ans;
char ch;
int main() {
	cin >> r1 >> c1 >> r2 >> c2 >> m;
	r2 -= r1, c2 -= c1, ans = abs(r2) + abs(c2);
	while (m--) {
		cin >> ch;
		if (ch == 'E') r2 -= r2 > 0;
		if (ch == 'N') c2 -= c2 > 0;
		if (ch == 'W') r2 += r2 < 0;
		if (ch == 'S') c2 += c2 < 0;
		if (!r2 && !c2) {
			cout << ans << endl;
			return 0;
		}
	}
	return puts("-1"), 0;
}
