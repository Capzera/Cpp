#include <bits/stdc++.h>
using namespace std;
string s;
long long n, k;
int main() {
	cin >> s >> k;
	n = s.size();
	while (n < k) {
		long long i = n;
		while (k > i * 2) i <<= 1;
		k -= i + 1;
		if (!k) k = i;
	}
	cout << s[k - 1];
	return 0;
}
