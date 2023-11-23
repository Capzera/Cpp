#include <bits/stdc++.h>
using namespace std;
int main() {
    char s[100010], c;
    string ss;
    int nxt[100010], cnt, tail, pre;
    while (cin >> ss) {
        memset(nxt, -1, sizeof(nxt));
        cnt = tail = pre = 0;
        int len = ss.size();
        for (int i = 0; i < len; i++) {
            c = ss[i];
            if (c == '[') pre = 0;
            else if (c == ']') pre = tail;
            else {
                s[++cnt] = c;
                nxt[cnt] = nxt[pre];
                nxt[pre] = cnt;
                if (pre == tail) tail = cnt;
                pre = cnt;
            }
        }
        for (int i = nxt[0]; ~i; i = nxt[i]) {
            cout << s[i];
        }
        cout << endl;
    }
    system("pause");
    return 0;
}