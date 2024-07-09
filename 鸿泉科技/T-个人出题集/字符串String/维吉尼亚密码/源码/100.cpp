#include <bits/stdc++.h>
using namespace std;
int main() {
	string s1, s2;
	cin >> s1 >> s2;
	int n = s1.size(), m = s2.size();
	for (int i = 0, j = 0; i < n; i++, j = (j + 1) % m) {
		s1[i] = (s1[i] - 'a' + s2[j] - 'a') % 26 + 'a';
	}
	cout << s1;
	return 0;
}
