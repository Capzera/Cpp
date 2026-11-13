#include <bits/stdc++.h>
using namespace std;
long long n, f1 = 1, f2 = 0, f = 1;
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		f = f1 + f2;
		f2 = f1;
		f1 = f;
	}
	cout << f;
	return 0;
}
