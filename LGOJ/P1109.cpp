#include <bits/stdc++.h>
using namespace std;
string a, b;
string add(string a, string b);
string sub(string a, string b);
int main() {
	cin >> a >> b;
	cout << sub(a, b);
	return 0;
}
string add(string a, string b) {
	if (a == "0") return b;
	if (b == "0") return a;
	if (a[0] != '-' && b[0] == '-') return sub(a, b.substr(1));
	if (a[0] == '-' && b[0] != '-') return sub(b, a.substr(1));
	if (a[0] == '-' && b[0] == '-') return "-" + add(a.substr(1), b.substr(1));
	string ans;
	int l1 = a.size() - 1, l2 = b.size() - 1, x = 0;
	while (~l1 || ~l2) {
		int n1 = l1 >= 0 ? a[l1--] - '0' : 0;
		int n2 = l2 >= 0 ? b[l2--] - '0' : 0;
		x += n1 + n2;
		ans += x % 10 + '0';
		x /= 10;
	}
	if (x) ans += "1";
	reverse(ans.begin(), ans.end());
	return ans;
}
string sub(string a, string b) {
	if (a[0] == '-' && b[0] != '-') return "-" + add(a.substr(1), b);
	if (a[0] != '-' && b[0] == '-') return add(a, b.substr(1));
	if (a[0] == '-' && b[0] == '-') return sub(b.substr(1), a.substr(1));
	if (a.size() < b.size() || a.size() == b.size() && a < b) return "-" + sub(b, a);
	string ans;
	int l1 = a.size() - 1, l2 = b.size() - 1, x = 0;
	while (~l1 || ~l2) {
		int n1 = l1 >= 0 ? a[l1--] - '0' : 0;
		int n2 = l2 >= 0 ? b[l2--] - '0' : 0;
		x = n1 - n2 - x + 10; // 默认借位
		ans += x % 10 + '0';
		x = x / 10 ^ 1;
	}
	while (ans.size() > 1 && ans.back() == '0') ans.pop_back();
	reverse(ans.begin(), ans.end());
	return ans;
}
