#include <bits/stdc++.h>
using namespace std;
long long n, x;
string add(string s1, string s2) {
	string ans;
	int l = s1.size() - 1, r = s2.size() - 1, carry = 0;
	while (l >= 0 || r >= 0) {
		int a = (l >= 0 ? s1[l--] - '0' : 0);
		int b = (r >= 0 ? s2[r--] - '0' : 0);
		carry += a + b;
		ans += carry % 10 + '0';
		carry /= 10;
	}
	if (carry) ans += "1";
	reverse(ans.begin(), ans.end());
	return ans;
}
int main() {
	cin >> n >> x;
	string a(n + 1, '0'), b = to_string(x);
	a[0]++;
	cout << add(a, b);
	return 0;
}
/*
int a, b, c;
if (a > b) {
	c = 5;
} else {
	c = 10;
}
c = a > b ? 5 : 10;
变量 = 条件 ? 条件成立值 : 不成立值


if (flag) {
	cout << "Yes << endl;
	return 0;
} else {
	cout << "No" << endl;
	return 0;
}

cout << (flag ? "Yes" : "No") << endl;
return 0;
*/
