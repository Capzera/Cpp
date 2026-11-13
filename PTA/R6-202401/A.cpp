#include <bits/stdc++.h>
using namespace std;
int x;
string s;
int main() {
	cin >> x >> s;
	for (auto& ch : s) {
		putchar((ch - 'a' + x) % 26 + 'a');
	}
	return 0;
}

