#include <bits/stdc++.h>
using namespace std;
long long n;
bool check() {
	if (n % 172800 == 0) return 1;
	if (n % 3200 == 0 && n % 172800) return 0;
	if (n % 400 == 0 && n % 3200) return 1;
	if (n % 100 == 0 && n % 400) return 0;
	if (n % 4 == 0 && n % 100) return 1;
	return 0;
}
int main() {
	cin >> n;
	cout << (check() ? "Yes" : "No");
	return 0;
}
