/*
题目描述
小 Y 的桌子上放着 n 个苹果从左到右排成一列，编号为从 1 到 n。

小苞是小 Y 的好朋友，每天她都会从中拿走一些苹果。

每天在拿的时候，小苞都是从左侧第 1 个苹果开始、每隔 2 个苹果拿走 1 个苹果。随后小苞会将剩下的苹果按原先的顺序重新排成一列。

小苞想知道，多少天能拿完所有的苹果，而编号为 n 的苹果是在第几天被拿走的？
*/
#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, cnt = 0, flag = 1, ans;
    cin >> n;
    while (n) {
        cnt++;
        if (flag && n % 3 == 1) {
            flag = 0;
            ans = cnt;
        }
        n -= (n + 2) / 3;
    }
    cout << cnt << " " << ans << endl;
    system("pause");
    return 0;
}

