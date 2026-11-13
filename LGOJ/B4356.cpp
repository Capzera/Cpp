#include <bits/stdc++.h>
using namespace std;
long long n, ans;
int main() {
	cin >> n;
	int m = (n + 1) / 2, odd = m * (m + 1) / 2;
	cout << n * (n + 1) / 2 - odd << "\n";
	return 0;
}
