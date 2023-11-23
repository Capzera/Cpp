#include <bits/stdc++.h>
using namespace std;
int main() {
    string s1, s2;
    cin >> s1 >> s2;
    vector<int> month = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int y1 = stoi(s1.substr(0, 4)), ans = 0;
    int y2 = stoi(s2.substr(0, 4)), m2 = stoi(s2.substr(4, 2)), d2 = stoi(s2.substr(6, 2));
    for (; y1 <= y2; y1++) {
        month[2] = 28;
        if (y1 % 400 == 0 || (y1 % 4 == 0 && y1 % 100 != 0)) month[2] = 29;
        string ss = to_string(y1), re = ss;
        reverse(re.begin(), re.end());
        int mm = stoi(re.substr(0, 2)), dd = stoi(re.substr(2, 2));
        if (y1 < y2 && mm < 13 && dd <= month[mm]) ans++;
        else if (y1 == y2) {
            if (mm > m2) break;
            if (mm < m2) ans++;
            else ans += dd <= d2;
        }
    }
    cout << ans << endl;
    system("pause");
    return 0;
}
/*
19970502
20161031

20011002
20100102
20111102

*/