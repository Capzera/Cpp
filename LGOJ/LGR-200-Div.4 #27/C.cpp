#include <bits/stdc++.h>
using namespace std;
long long n;
double m;
int main() {
	cin >> n >> m;
	if (!m) return puts("1"), 0;
	long long x = (1.0 * m * (n * (n + 1) / 2) + 99) / 100;
	cout << (x > n ? -1 : x);
	return 0;
}
