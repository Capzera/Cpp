#include <bits/stdc++.h>
using namespace std;
string s;
long long n, suf, a[100001];
int main() {
	cin >> n >> s;
	for (int i = 0; i < n; i++) {
		cin >> a[i];
		if (i % 2 == 0) a[i] = -a[i];
	}
	for (int i = n - 1; ~i; i--) {
		suf += a[i];
		s[i] = ((s[i] - 'a' + suf) % 26 + 26) % 26 + 'a';
	}
	cout << s;
	return 0;
}
/*
((原始编号 + 位移量) % 环大小 + 环大小) % 环大小
((p + x) % n + n) % n
*/
