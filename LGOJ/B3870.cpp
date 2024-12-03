#include <bits/stdc++.h>
using namespace std;
long long n;
string mp = "0123456789ABCDEF";
void get(int x) {
	printf("%c%c ", mp[x / 16], mp[x % 16]);
}
int main() {
	cin >> n;
	if (!n) return puts("00"), 0;
	while (n) {
		int k = n % 128;
		n /= 128;
		if (n) k += 128;
		get(k);
	}
	return 0;
}
