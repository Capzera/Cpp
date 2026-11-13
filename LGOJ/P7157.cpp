#include <bits/stdc++.h>
using namespace std;
long long n, k;
int main() {
	cin >> n >> k;
	cout << (n - 1) * n / 2 - k;
	return 0;
}
