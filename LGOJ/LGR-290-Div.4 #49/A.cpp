#include <bits/stdc++.h>
using namespace std;
long long n, x, y, row, num;
int main() {
	cin >> n >> x >> y;
	while (n--) {
		cin >> num;
		row += (num + 1 + x) / x;
	}
	cout << (row + y - 1) / y;
	return 0;
}
