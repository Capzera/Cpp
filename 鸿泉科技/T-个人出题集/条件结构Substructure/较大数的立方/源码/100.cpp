#include <bits/stdc++.h>
using namespace std;
int main() {
	long long a, b;
	cin >> a >> b;
	long long a3 = a * a * a, b3 = b * b * b;
	if (a > b) {
		cout << a3 << endl;
	} else cout << b3 << endl;
	return 0;
}
