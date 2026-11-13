#include <bits/stdc++.h>
using namespace std;
int main() {
	string s;
	int k;
	cin >> s >> k;
	for (auto& x : s) {
		x = ((x - '0') % k) + '0';
	}
	cout << stoi(s);
	return 0;
}
