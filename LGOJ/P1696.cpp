#include<iostream>
using namespace std;
int r1, c1, r2, c2, r3, c3, r4, c4;
int main() {
	cin >> r1 >> c1 >> r2 >> c2 >> r3 >> c3 >> r4 >> c4;
	if (r3 <= r1 && r2 <= r4 && c3 <= c1 && c2 <= c4)cout << 0, exit(0);
	if (r1 < r3 && r3 < r2 && r2 <= r4 && c3 <= c1 && c2 <= c4) cout << (r3 - r1) * (c2 - c1), exit(0);
	if (r3 <= r1 && r2 <= r4 && c1 < c3 && c3 < c2 && c2 <= c4) cout << (r2 - r1) * (c3 - c1), exit(0);
	if (r3 <= r1 && r1 < r4 && r4 < r2 && c3 <= c1 && c2 <= c4) cout << (r2 - r4) * (c2 - c1), exit(0);
	if (r3 <= r1 && r2 <= r4 && c3 <= c1 && c1 < c4 && c4 < c2) cout << (r2 - r1) * (c2 - c4), exit(0);
	cout << (r2 - r1) * (c2 - c1);
	return 0;
}
/*
r1 < r3 < r2 <= r4
c3 < c1  c2 < c4
r3 <= r1 r2 <= r4
c1 < c3 < c2 <= c4

r3 <= r1 < r4 < r2
c1 <= c1  c2 <= c4

*/
