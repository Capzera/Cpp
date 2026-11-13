#include <bits/stdc++.h>
using namespace std;
int main() {
    string s = "tocyjkdzcieoiodfpbgcncsrjbhmugdnojjddhllnofawllbhfiadgdcdjstemphmnjihecoapdjjrprrqnhgccevdarufmliqijgihhfgdcmxvicfauachlifhafpdccfseflcdgjncadfclvfmadvrnaaahahndsikzssoywakgnfjjaihtniptwoulxbaeqkqhfwl";
    int n = s.size(), ans = 0;
    vector<int> dp(n, 1);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (s[i] > s[j]) dp[i] += dp[j];
            if (s[i] == s[j]) dp[i] = 0;
        }
        ans += dp[i];
    }
    cout << ans << endl;
    system("pause");
    return 0;
}
/*
3616159
*/