#include <bits/stdc++.h>
using namespace std;
long long n;
int main() {
	cin >> n;
	if (n & 1) return puts("-1"), 0;
	for (int i = 24; ~i; i--) {
		if (n >> i & 1) cout << (1 << i) << " ";
	}
	return 0;
}
