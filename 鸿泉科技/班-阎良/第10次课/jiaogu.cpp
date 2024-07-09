#include <bits/stdc++.h>
using namespace std;
void f(long long n) {
	if (n == 1) {
		cout << "End" << endl;
		return;
	}
	if (n & 1) {
		cout << n << "*3+1=" << n * 3 + 1 << endl;
		f(n*3+1);
	}
	else {
		cout << n << "/2=" << n / 2 << endl;
		f(n/2);
	}
}
int main() {
	long long n;
	cin >> n;
	f(n);
	return 0;
}
