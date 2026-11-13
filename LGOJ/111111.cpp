#include <bits/stdc++.h>
using namespace std;
string ori, key;
string decode(string s, string key) {
	int n = s.size(), m = key.size();
	for (int i = 0, j = 0; i < n; i++, j = (j + 1) % m) {
		int xx = s[i] - key[j];
		s[i] = (xx + 128) % 128;
	}
	return s;
}
int main() {
	cin >> ori >> key;
	cout << decode(ori, key);
	return 0;
}
