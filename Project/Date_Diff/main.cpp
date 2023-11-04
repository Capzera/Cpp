/********************************************
*	> Project Name :	Date_Diff
*	> File Name    :   	main.cpp
*	> Author       :   	Capzera
*	> QQ-Mail      :   	1786126188@qq.com
*	> Create Time  :	2023-11-02 16:33:47
*   > Tips		   :	None
*********************************************/
#include <bits/stdc++.h>
#include <vector>
#include <math.h>
#include <algorithm>
using namespace std;
vector<int> month = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
int solve(int yy, int mm, int dd) {
    int ans = dd;
    for (int y = 1; y < yy; y++) {
        if ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0) ans++;
        ans += 365;
    }
    for (int m = 1; m < mm; m++) {
        ans += month[m];
    }
    return ans;
}
using namespace std;
int main() {
    int y1, m1, d1, y2, m2, d2;
    cin >> y1 >> m1 >> d1;
    cin >> y2 >> m2 >> d2;
    cout << solve(y1, m1, d1) << endl;
    cout << abs(solve(y1, m1, d1) - solve(y2, m2, d2)) << endl;
    return 0;
}
