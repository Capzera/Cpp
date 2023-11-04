#include <bits/stdc++.h>
using namespace std;
int main() {
    string exp, cur = "";
    cin >> exp; exp += ' ';
    char ansC = 'a';
    int n = exp.size(), con = 0, coe = 0, ke = 1, kc = -1, op = 1;
    for (auto& ch : exp) {
        if (isdigit(ch)) {
            cur += ch;
            continue;
        }
        if (isalpha(ch)) {
            if (cur == "") cur = "1";
            coe += ke * op * stoi(cur);
            ansC = ch;
        }
        else if (cur != "") {
            con += kc * op * stoi(cur);
        }
        cur = "";
        if (ch == '-') op = -1;
        if (ch == '+') op = 1;
        if (ch == '=') ke = -1, kc = 1, op = 1;
    }
    double ans = !con ? 0 : (1.0 * con / coe);
    cout << ansC << "=" << fixed << setprecision(3) << ans << endl;
    system("pause");
    return 0;
}