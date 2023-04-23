#include <bits/stdc++.h>
using namespace std;
int main() {
    cin.tie(nullptr)->sync_with_stdio(0);
    int n, num;
    int black = 0, white = 0, sum = 0;
    vector<int> cnt = {1, 0};
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> num;
        sum ^= num > 0 ? 0 : 1;
        black += cnt[sum ^ 1];
        white += cnt[sum];
        cnt[sum]++;
    }
    cout << black << " " << white << endl;
    system("pause");
    return 0;
}
/*
给一段数组，求多少个连续的子数组乘积大于0，多少个子数组乘积小于0;
如果一段区间负数为偶数个，乘积大于0，如果为奇数个乘积小于0
5 -3 3 -1 1
  0 1 0 1 0
0 0 1 1 0 0
[l, r]
sum[r + 1] - sum[l] 是偶数，[l, r]这个区间乘积为正
0 ^ 0 = 0
0 ^ 1 = 1
1 ^ 0 = 1
1 ^ 1 = 0
异或和
【1,3】
sum[4] = 0  sum[1] = 0
sum[2] = 1 sum[3] = 1
l = 2 r + 1 = 3
[l ,r] 偶数
如果sum[r + 1] = sum[l] 说明[l, r]乘积为正
如果sum[r + 1] != sum[l] 说明[l , r] 乘积为负
前缀和 + （哈希表） = O(N)
异或和

0 1 0 1 0
0 1 1 0 0
对黑魔法来说（乘积为负），当前异或和为1， 要找先前0，当前异或和为0，要找先前1
对白魔法来说（乘积为正），当前异或和为0， 要找先前0，当前异或和为1，要找先前1
已有0：3
已有1：2
黑：0 + 2 + 2 + 2 + 2 = 8
白：1 + 0 + 1 + 2 + 3 = 7

（0初始情况为1）
计算当前的异或和
分别计算黑白魔法的结果
将当前结果统计进哈希表


求一个区间的和是不是偶数
*/