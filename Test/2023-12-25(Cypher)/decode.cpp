#include <bits/stdc++.h>
using namespace std;
string itoc(int& x) {
    string ans(8, '0');
    for (int i = 1, mask = 64; x; i++, mask /= 2) {
        if (x < mask) continue;
        x -= mask;
        ans[i] = '1';
    }
    return ans;
}
int btoc(string bit) {
    int ans = 0;
    for (int i = 1, mask = 64; i < 8; i++, mask >>= 1) {
        if (bit[i] == '0') continue;
        ans += mask;
    }
    return ans;
}
void decode(string& text, string& okey) {
    int n1 = okey[0], n2 = okey[1];
    char t, t2;
    string key = itoc(n1) + itoc(n2);
    for (int i = 0; i < 8; i++) {
        text[i << 1] = (text[i << 1] - '0') ^ (key[i << 1] - '0') + '0';
        text[i << 1 | 1] = (text[i << 1 | 1] - '0') ^ (key[i << 1 | 1] - '0') + '0';
        swap(text[i << 1], text[i << 1 | 1]);
    }
    t = text[7], text[7] = text[6], text[6] =text[5], text[5] = text[4], text[4] = t;
    t = text[10], t2 = text[11], text[10] = text[8], text[11] = text[9], text[8] = t, text[9] = t2;
    t = text[12], text[12] = text[13], text[13] = text[14], text[14] = text[15], text[15] = t;
}
int main() {
    int op;
    string origin, r1, r2;
    cin >> origin >> op >> r1 >> r2;
    if (op == 2) {
        decode(origin, r1);
        decode(origin, r2);
        char a = btoc(origin.substr(0, 8)), b = btoc(origin.substr(8, 8));
        cout << a << b << endl;
    }
    else {
        encode()
    }
    system("pause");
    return 0;
}
/*
1001011110110101 2
YS
BX

FN 1
YS BX
*/