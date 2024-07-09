#include <iostream>
#include <string>
#include <algorithm>
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
int main() {
	string a1, b1, a2, b2; 
	int k;
	cin >> a1 >> b1 >> k;
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
	string x1 = add(a1, a2), x2 = add(b1, b2);
	for (int i = 0; i < k; i++) {
		x1 = add(x1, a1);
		x2 = add(x2, b1);
	}
	cout << sub(x1, x2);
	return 0;
}

