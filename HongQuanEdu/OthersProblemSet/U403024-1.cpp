#include <bits/stdc++.h>
using namespace std;
int main() {
    string str, tmp;
    cin >> str;
    int n = str.size(), flag = 0;
    for (auto& ch : str) ch--;
    for (auto& ch : str) {
        if (!flag && isalpha(ch)) {
            ch -= 32;
            flag = 1;
        }
        if (ch == '.') {
            cout << "    " << tmp << '.' << endl;
            flag = 0;
            tmp = "";
            continue;
        }
        tmp += ch;
    }
    system("pause");
    return 0;
}