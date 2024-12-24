#include <bits/stdc++.h>
using namespace std;
int main() {
	long long n;
	cin >> n;
	while (n) {
		int rest = n % 2;
		n /= 2;
		cout << n << " " << rest << endl;
	}
	return 0;
}
