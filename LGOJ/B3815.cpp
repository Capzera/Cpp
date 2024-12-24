#include <bits/stdc++.h>
using namespace std;
long long n, x, sum;
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> x;
		sum += 21 - x;
		if (x == 1) sum -= 2;
		if (x == 2) sum -= 1;
		if (x == 3) sum -= 4;
		if (x == 4) sum -= 3;
		if (x == 5) sum -= 6;
		if (x == 6) sum -= 5;
	}
	cout << sum + x;
	return 0;
}
