#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, x;
	cin >> n;
	int d5 = 0, d10 = 0, i = 1;
	for (; i <= n; i++) {
		cin >> x;
		if (x == 5) d5++;
		else if (x == 10) d5--, d10++;
		else if (d10) d10--, d5--;
		else d5 -= 3;
		if (d5 < 0) break;
	} 
	cout << (i > n ? "YES" : to_string(i)) << endl;
	return 0;
}

