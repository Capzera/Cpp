#include <bits/stdc++.h>
using namespace std;
string add(string s1, string s2) {
    string ans;
    int l = s1.size() - 1, r = s2.size() - 1, carry = 0;
    while (l >= 0 || r >= 0) {
        int a = l >= 0 ? s1[l--] - '0' : 0;
		int b = r >= 0 ? s2[r--] - '0' : 0;
        carry += a + b;
        ans += carry % 10 + '0';
        carry /= 10;
    }
    if (carry) ans += "1";
    reverse(ans.begin(), ans.end());
    return ans;
}
string sub(string s1, string s2) {
    string ans;
    int l = s1.size() - 1, r = s2.size() - 1, carry = 0;
    if (l < r || l == r && s1 < s2) {
    	swap(s1, s2);
    	swap(l, r);
	}	
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
string mul(string s1, string s2) {
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
        if (i) ans[i - 1] += x;
    }
    if (x) ans = to_string(x) + ans;
    return ans;
}
int main() {
    string a1, b1, a2, b2, k; 
	cin >> a1 >> b1 >> k; k = add(k, "1");
	int i = 1, j = 1;
	while (i < a1.size() && a1[i] != '.') {
		i++;
	} 
	while (j < b1.size() && b1[j] != '.') {
		j++;
		
	}
	a2 = a1.substr(i + 1);
	a1 = a1.substr(1, i - 1);
	b2 = b1.substr(j + 1);
	b1 = b1.substr(1, j - 1);
	string x1 = add(mul(a1, k), a2);
	string x2 = add(mul(b1, k), b2);
	cout << sub(x1, x2);
    return 0;
}
