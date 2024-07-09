#include <bits/stdc++.h>
using namespace std;
string add(string s1, string s2) {
    string ans = "";
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
int main() {
    int n;
    cin >> n;
    string a = "1", b = "2", c = "2";
    for (int i = 2; i <= n; i++) {
    	c = add(a, b);
    	a = b;
    	b = c;
	}
    cout << c << endl;
    return 0;
}
