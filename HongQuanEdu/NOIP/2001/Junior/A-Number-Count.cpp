/*
给出正整数 n，要求按如下方式构造数列：

只有一个数字 n 的数列是一个合法的数列。
在一个合法的数列的末尾加入一个正整数，但是这个正整数不能超过该数列最后一项的一半，可以得到一个新的合法数列。
请你求出，一共有多少个合法的数列。两个合法数列 a, b 不同当且仅当两数列长度不同或存在一个正整数 i≤∣a∣，使得 a[i] <= b[i]
*/
#include <bits/stdc++.h>
using namespace std;
int main() {
    int n;
    cin >> n;
    vector<int> dp(n + 1);
    for (int i = 1; i <= n; i++) {
        for (int j = 0; j <= i / 2; j++) {
            dp[i] += dp[j];
        }
        dp[i]++;
    }
    cout << dp[n] << endl;
    system("pause");
    return 0;
}