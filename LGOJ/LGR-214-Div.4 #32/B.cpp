#include <bits/stdc++.h>
using namespace std;
int n, x, y, a, b;
int main() {
	cin >> n >> x >> y;
	a = y > x ? y - x : n - x + y;
	b = y > x ? n - y + x : x - y;
	if (a < b) {
		cout << "Clockwise Loop";
	} else if (a > b) {
		cout << "Counter-clockwise Loop";
	} else cout << "\"Wonderful\"";
	return 0;
}
