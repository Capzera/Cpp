#include <bits/stdc++.h>
using namespace std;
const int p = 1e9 + 7;
long long n;
int main() {
	cin >> n;
	cout << n * n % p * (n + 1) % p * (n - 1) % p * 83333334 % p;
	return 0;
}

