#include <bits/stdc++.h>
using namespace std;
int t, x, y, z, ans;
int cal(int x, int y) {
	int c = 0;
	while (c < z) {
		if ((x + y) & 1) x -= y % x;
		else y -= x % y;
		if (x < c) x += y / 2 + 1;
		if (y < c) y += x / 2 + 1;
		c++;
	}
	return x == ::x && y == ::y;
}
int main() {
	cin >> x >> y >> z;
	for (int i = 10; i < 100; i++)
		for (int j = 10; j < 100; j++)
			ans += cal(i, j);
	cout << ans;
	return 0;
}
