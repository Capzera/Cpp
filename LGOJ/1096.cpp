#include <bits/stdc++.h>
using namespace std;
int n;
string HAMulti(string s1, string s2) {
	if (s1 == "0" || s2 == "0") return "0";
	int l1 = s1.size(), l2 = s2.size(), x;
	string ans(l1 + l2 - 1, '0');
	for (int i = l1 - 1; i >= 0; i--) {
		x = 0;
		for (int j = l2 - 1; j >= 0; j--) {
			int a = s1[i] - '0', b = s2[j] - '0';
			x += a * b + ans[i + j] - '0';
			ans[i + j] = x % 10 + '0';
			x /= 10;
		}
		if (i) {
			ans[i - 1] += x;
		}
	}
	if (x) {
		ans = to_string(x) + ans;
	}
	return ans;
}
string sub(string s1, string s2, int minus) {
	string ans;
	int l = s1.size() - 1, r = s2.size() - 1, carry = 0;
	if (l < r || l == r && s1 < s2) return sub(s2, s1, 1);
	if (minus) cout << '-';
	while (l >= 0 || r >= 0) {
		int a = l < 0 ? 0 : s1[l--] - '0';
		int b = r < 0 ? 0 : s2[r--] - '0';
		carry = a + 10 - b - carry;
		ans += carry % 10 + '0';
		carry = carry / 10 ^ 1;
	}
	while (ans.size() > 1 && ans.back() == '0') ans.pop_back();
	reverse(ans.begin(), ans.end());
	return ans;
}
int main() {
	cin >> n;
	string base = "2";
	for (int i = 1; i <= n; i++) {
		base = HAMulti(base, "2");
	}
	base = sub(base, "2", 0);
	cout << base;
	return 0;
}
