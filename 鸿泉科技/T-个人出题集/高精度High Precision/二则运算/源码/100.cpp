#include <bits/stdc++.h>
using namespace std;
string sub(string s1, string s2, int minus) {
    string ans;
    int l = s1.size() - 1, r = s2.size() - 1, carry = 0;
    if (l < r || (l == r && s1 < s2)) return sub(s2, s1, 1);
    while (l >= 0 || r >= 0) {
        int a = l < 0 ? 0 : s1[l--] - '0';
        int b = r < 0 ? 0 : s2[r--] - '0';
        carry = a + 10 - b - carry;
        ans += carry % 10 + '0';
        carry = carry / 10 ^ 1;
    }
    while (ans.size() > 1 && ans.back() == '0') ans.pop_back();
    if (minus) ans += '-';
    reverse(ans.begin(), ans.end());
    return ans;
}
string add(string s1, string s2) {
    string ans;
    bool f1 = 0, f2 = 0, f3 = 0;
    if (s1[0] == '-') {
    	f1 = 1;
    	s1 = s1.substr(1);
	} 
    if (s2[0] == '-') {
    	f2 = 1;
    	s2 = s2.substr(1);
	}
    if (f1 && !f2) return sub(s2, s1, 0);
	if (f2 && !f1) return sub(s1, s2, 0);
	f3 = f1 & f2;
    int l = s1.size() - 1, r = s2.size() - 1, carry = 0;
    while (l >= 0 || r >= 0) {
        int a = l >= 0 ? s1[l--] - '0' : 0;
		int b = r >= 0 ? s2[r--] - '0' : 0;
        carry += a + b;
        ans += carry % 10 + '0';
        carry /= 10;
    }
    if (carry) ans += "1";
    if (f3) ans += "-";
    reverse(ans.begin(), ans.end());
    return ans;
}
string mul(string s1, string s2) {
    if (s1 == "0" || s2 == "0") return "0";
    bool f1 = 0, f2 = 0, f3 = 0;
	if (s1[0] == '-') {
		f1 = 1;
		s1 = s1.substr(1);
	}
    if (s2[0] == '-') {
    	f2 = 1;
    	s2 = s2.substr(1);
	}
    f3 = f1 ^ f2;
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
        if (i) ans[i - 1] += x;
    }
    if (x) ans = to_string(x) + ans;
    if (f3) ans = "-" + ans;
    return ans;
}
int main() {
	string s1, s2, s3;
	cin >> s1 >> s2 >> s3;
	cout << mul(add(s1, s2), s3);
	return 0;
} 
