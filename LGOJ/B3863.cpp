#include <bits/stdc++.h>
using namespace std;
int x, y, z, money, q;
int main() {
	cin >> x >> y >> z >> q;
	money = 2 * x + 5 * y + 3 * z;
	cout << (money <= q ? "Yes\n" : "No\n");
	cout << abs(money - q);
	return 0;
}
