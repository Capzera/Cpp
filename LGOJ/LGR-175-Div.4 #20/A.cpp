#include <bits/stdc++.h>
using namespace std;
long long x, y, z, ans;
int main() {
	cin >> x >> y >> z;
	ans += x > y + z;
	ans += y > x + z;
	ans += z > x + y;
	cout << ans;
	return 0;
}
