#include <bits/stdc++.h>
using namespace std;
int main() {
	string s = "";
	for (int i = 1; i <= 819; i++) {
		string num = to_string(i);
		for (int j = 1; j <= i; j++) s += num;
	}
	vector<int> sum(s.size() + 1);
	for (int i = 1; i <= s.size(); i++) {
		sum[i] = sum[i - 1] + s[i - 1] - '0';
	}
	int t, l, r;
	cin >> t;
	while (t--) {
		cin >> l >> r;
		if (l > r) swap(l, r);
		cout << sum[r] - sum[l - 1] << endl;
	}
	return 0;
}
