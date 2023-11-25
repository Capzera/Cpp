#include <bits/stdc++.h>
using namespace std;

string HAminus(string s1, string s2, int minus) {
    string ans;
    int l = s1.size() - 1, r = s2.size() - 1, carry = 0;
    if (l < r || l == r && s1 < s2) return HAminus(s2, s1, 1);
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

string HAMulti(string s1, string s2) {
    if (s1 == "0" || s2 == "0") return "0";
    int l1 = s1.size(), l2 = s2.size(), carry;
    string ans(l1 + l2 - 1, '0');
    for (int i = l1 - 1; i >= 0; i--) {
        carry = 0;
        for (int j = l2 - 1; j >= 0; j--) {
            carry += ans[i + j] - '0' + (s1[i] - '0') * (s2[j] - '0');
            ans[i + j] = carry % 10 + '0';
            carry /= 10;
        }
        if (carry && i) ans[i - 1] = carry + '0';
    }
    if (carry) ans = to_string(carry) + ans;
    return ans;
}

string HADivide(stirng s1, string s2) {
    int l1 = s1.size(), l2 = s2.size();
    string ans;
    
    return ans;
}

int main() {
    string s1, s2;
    cin >> s1 >> s2;
    cout << HADivide(s1, s2) << endl;
    system("pause");
    return 0;
}