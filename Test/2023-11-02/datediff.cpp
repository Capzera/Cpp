#include <bits/stdc++.h>
using namespace std;
vector<int> month = {0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
int isLunar(int y) {
    return (y % 4 == 0 && y % 100 != 0) || y % 400 == 0;
}
int solve(int yy, int mm, int dd) {
    int ans = dd;
    for (int y = 1; y < yy; y++) ans += isLunar(y) + 365;
    ans += isLunar(yy) && (mm > 2 || (mm == 2 && dd == 29));
    ans += month[mm];
    return ans;
}
using namespace std;
int main() {
    int y1, m1, d1, y2, m2, d2;
    cin >> y1 >> m1 >> d1 >> y2 >> m2 >> d2;
    cout << abs(solve(y1, m1, d1) - solve(y2, m2, d2)) << endl;
    system("pause");
    return 0;
}