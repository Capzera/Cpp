#include <bits/stdc++.h>
using namespace std;
int main(){
    vector<int> month = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int mm = 1, dd = 1, week = 5, ans = 0;
    while (1) {
        if (dd == 1 || dd == 11 || dd == 21 || dd == 31 || week == 5 || week == 6) ans++;
        dd++;
        week++;
        if (week == 7) week = 0;
        if (dd > month[mm]) {
            mm++;
            dd = 1;
        }
        if (mm == 13) break;
    }
    cout << ans << endl;
    return 0;
}
