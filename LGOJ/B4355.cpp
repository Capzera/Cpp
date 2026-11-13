#include <bits/stdc++.h>
using namespace std;
long long n, m;
int main() {
	cin >> n >> m;
	cout << n * m / __gcd(n, m);
	return 0;
}
