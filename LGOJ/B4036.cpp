#include <bits/stdc++.h>
using namespace std;
int n, x, ans;
string s;
int main() {
	cin >> n;
	while (n--) {
		cin >> s;
		int sum = 0;
		for (auto& ch : s) sum += ch - '0';
		cout << (sum % 7 == 0 ? "Yes\n" : "No\n");
	}
	return 0;
}
