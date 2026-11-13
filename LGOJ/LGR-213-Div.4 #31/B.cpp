#include <bits/stdc++.h>
using namespace std;
int x, y;
int get(int x) {
	if (x & 1) return 50 - (x >> 1);
	return (x >> 1) + 51;
}
int main() {
	cin >> x >> y;
	cout << abs(get(x) - get(y)) - 1;
	return 0;
}
