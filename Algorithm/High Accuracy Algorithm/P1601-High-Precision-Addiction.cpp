#include <bits/stdc++.h>
using namespace std;
string HAplus(string s1, string s2) {
    string ans;
    int l = s1.size() - 1, r = s2.size() - 1, carry = 0;
    while (l >= 0 || r >= 0) {
        int a = 0, b = 0;
        if (l >= 0) {
            a = s1[l];
            l--;
        }
        if (r >= 0) {
            b = s2[r];
            r--;
        }
        carry += a + b;
        ans += carry % 10 + '0';
        carry /= 10;
    }
    if (carry) ans += "1";
    reverse(ans.begin(), ans.end());
    return ans;
}
int main() {
    string s1, s2;
    cin >> s1 >> s2;
    cout << HAplus(s1, s2) << endl;
    system("pause");
    return 0;
}