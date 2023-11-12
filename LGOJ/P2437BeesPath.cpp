#include <bits/stdc++.h>
using namespace std;
string add(string& a, string& b) {
    int i = a.size() - 1, j = b.size() - 1, carry = 0;
    string ans = "";
    while (i >= 0 || j >= 0) {
        int n1 = i < 0 ? 0 : a[i--] - '0';
        int n2 = j < 0 ? 0 : b[j--] - '0';
        carry += n1 + n2;
        ans.push_back(carry % 10 + '0');
        carry /= 10;
    }
    if (carry) ans += "1";
    reverse(ans.begin(), ans.end());
    return ans;
}
int main() {
    int m, n;
    cin >> m >> n;
    string dp = "1", dp0 = "0";
    n = n - m;
    while (n--) {
        string dpt = dp;
        dp = add(dp, dp0);
        dp0 = dpt;
    }
    cout << dp << endl;
    system("pause");
    return 0;
}