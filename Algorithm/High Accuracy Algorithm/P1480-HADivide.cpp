#include <bits/stdc++.h>
using namespace std;
string HADivide(string s1, string s2) {
    int l1 = s1.size(), l2 = s2.size();
    if (l1 < l2 || l1 == l2 && s1 < s2) return "0";
    string ans, div = s1.substr(0, l2 - 1);
    for (int i = l2 - 1, j; i < l1; i++) {
        div += s1[i];
        for (j = 0; j <= 9; j++) {
            int l = div.size() - 1, r = l2 - 1, carry = 0;
            if (l < r || l == r && div < s2) break;
            while (r >= 0) {
                carry = 10 + div[l] - s2[r--] - carry;
                div[l--] = carry % 10 + '0';
                carry = carry / 10 ^ 1;
            }
            if (carry && div[0] != '0') div[0]--;
            while (div[0] == '0') div = div.substr(1);
        }
        if (j || ans.size()) ans += '0' + j;
    }
    return ans;
}
int main() {
    string s1, s2;
    cin >> s1 >> s2;
    cout << HADivide(s1, s2) << endl;
    system("pause");
    return 0;
}