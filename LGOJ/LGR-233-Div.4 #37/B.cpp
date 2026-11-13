#include <bits/stdc++.h>
using namespace std;
long long n, a, b;
int main() {
	cin >> n >> a >> b;
	cout << (a * n < 0 ? b : min(a * n, b));
	return 0;
}
