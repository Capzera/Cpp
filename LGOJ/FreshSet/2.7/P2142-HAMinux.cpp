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
int main() {
    string s1, s2;
    cin >> s1 >> s2;
    cout << HAminus(s1, s2, 0) << endl;
    system("pause");
    return 0;
}