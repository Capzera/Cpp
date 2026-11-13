#include <bits/stdc++.h>
using namespace std;
int main() {
    string str;
    getline(cin, str);
    int n = str.size(), ans = -1, cnt = 1, start = 0;
    for (int i = 1; i < n; i++) {
        if (str[i] - str[i - 1] != 1) {
            if (cnt > ans) {
                ans = cnt;
                start = i - ans;
            }
            cnt = 0;
        }
        cnt++;
    }
    if (cnt > ans) {
        ans = cnt;
        start = n - ans;
    }
    cout << str.substr(start, ans) << endl;
    system("pause");
    return 0;
}
/*
input : 
abcd12345ed125ss123456789

*/