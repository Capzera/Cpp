#include <bits/stdc++.h>
using namespace std;
string HADivide(string s1, string s2) {
    int l1 = s1.size(), l2 = s2.size();
    if (l1 < l2 || l1 == l2 && s1 < s2) return "0";
    string ans, div = s1.substr(0, l2 - 1);
    for (int i = l2 - 1, j; i < l1; i++) {
        if (div == "0") div = "";
        div += s1[i];
        for (j = 0; j <= 9; j++) {
            int l = div.size(), r = l2 - 1, carry = 0;
            string ret;
            if (l < l2 || l == l2 && div < s2) break;
            l--;
            while (l >= 0 || r >= 0) {
                int a = l < 0 ? 0 : div[l--] - '0';
                int b = r < 0 ? 0 : s2[r--] - '0';
                carry = 10 + a - b - carry;
                ret += carry % 10 + '0';
                carry = carry / 10 ^ 1;
            }
            while (ret.size() > 1 && ret.back() == '0') ret.pop_back();
            reverse(ret.begin(), ret.end());
            div = ret;
        }
        if (!j && ans.empty()) continue;
        ans.push_back(j + '0');
    }
    return ans;
}
int main() {
    string s1, s2;
    cin >> s1 >> s2;
    cout << HADivide(s1, s2) << endl;
    return 0;
}
