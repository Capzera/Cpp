#include <bits/stdc++.h>
using namespace std;
long long n, m;
int main() {
	cin >> n >> m;
	if (n < 3) return puts("Impossible"), 0;
	if (n >= 8 || (n >= 6 && m)) return puts("3"), 0;
	cout << 6;
	return 0;
}
