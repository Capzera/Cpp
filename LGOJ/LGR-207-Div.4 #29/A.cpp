#include <bits/stdc++.h>
using namespace std;
int t, a, b, c, d;
int main() {
	cin >> t >> a >> b >> c >> d;
	cout << (!t ? a + b : max(a - c, 0) + max(b - d, 0));
	return 0;
}
