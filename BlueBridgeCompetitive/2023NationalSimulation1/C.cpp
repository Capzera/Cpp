#include <bits/stdc++.h>
using namespace std;
int main() {
    vector<int> moon = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    set<int> square = {1, 4, 9, 16, 25, 36, 49};
    auto cnt = [](int n) -> int {
        int ans = 0;
        while (n) {ans += n % 10; n /= 10;}
        return ans;
    };
    int ans = 0;
    for (int yy = 2001; yy <= 2021; yy++) {
        if (yy % 4 == 0) moon[2] = 29;
        else moon[2] = 28;
        for (int mm = 1; mm <= 12; mm++) {
            int sum = cnt(yy) + cnt(mm);
            for (int dd = 1; dd <= moon[mm]; dd++) {
                ans += square.count(sum + cnt(dd));
            }
        }
    }
    cout << ans << endl;
    system("pause");
    return 0;
}
/*
如果一个日期中年月日的各位数字之和是完全平方数，则称为一个完全日期。
例如：20212021 年 66 月 55 日的各位数字之和为 2 + 0 + 2 + 1 + 6 + 5 = 162+0+2+1+6+5=16，而 1616 是一个完全平方数，它是 44 的平方。所以 20212021 年 66 月 55 日是一个完全日期。
例如：20212021 年 66 月 2323 日的各位数字之和为 2 + 0 + 2 + 1 + 6 + 2 + 3 = 162+0+2+1+6+2+3=16，是一个完全平方数。所以 20212021 年 66 月 2323 日也是一个完全日期。
请问，从 20012001 年 11 月 11 日到 20212021 年 1212 月 3131 日中，一共有多少个完全日期？
*/