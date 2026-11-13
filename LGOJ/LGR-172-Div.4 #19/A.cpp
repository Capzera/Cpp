#include <bits/stdc++.h>
using namespace std;
long long x;
int main() {
	cin >> x;
	if (x > 400) return puts("1000"), 0;
	if (x > 300) return puts("400"), 0;
	if (x > 150) return puts("300"), 0;
	if (x > 100) return puts("150"), 0;
	cout << "100";
	return 0;
}
