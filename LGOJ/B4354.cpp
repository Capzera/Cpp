#include <bits/stdc++.h>
using namespace std;
long long n, k, t;
int main() {
	cin >> n >> k >> t;
	cout << min(n, k * t);
	return 0;
}
