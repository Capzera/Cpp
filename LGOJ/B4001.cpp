#include <bits/stdc++.h>
using namespace std;
long long n;
int main() {
	cin >> n;
	for (long long i = 1; i * i * i <= n; i++) {
		if (i * i * i == n) return puts("Yes"), 0;
	}
	cout << "No";
	return 0;
}
