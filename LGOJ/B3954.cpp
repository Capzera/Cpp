#include <bits/stdc++.h>
using namespace std;
long long n, x, sum = 1;
int main() {
	cin >> n;
	while (n--) {
		cin >> x;
		sum *= x;
		if (sum > 1e6) return puts(">1000000"), 0;
	}
	cout << sum;
	return 0;
}
