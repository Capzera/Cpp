/*
问题描述：
lxd是如此的强, 以至于他拿到过很多的奖杯, 强迫症的他把所有奖杯按顺序摆在了柜子上. 
这天, lxd想找一下自己某个名次的奖杯拿过多少次, 可是奖杯太多了实在找不过来, 于是他找到了你, 请你帮他找一找
输入
第一行包括两个整数 n 和 q, n表示lxd总共拥有的奖杯数量, q表示q次询问. (1 <= n <= 10^6, 1 <= q <= 1000)
第二行包括 n 个整数, a1, a2, ..., an (1 <= ai <= 10^9), 分别代表第 i 个奖杯是 ai 等奖. 
(例如: 当n=3时, 若给出序列1, 9, 100, 则代表第一个奖杯是1等奖, 第二个奖杯是9等奖, 第3个奖杯是100等奖...)
接下来的 q 行, 每行包括一个整数 x , 表示lxd想知道 x 等奖有多少个
输出
共 q 行, 每行一个整数, 代表所询问的 x 等奖有多少个, 若不存在, 则输出-1
输入样例 1 
5 3
1 1 3 7 18
1
7
100
输出样例 1
2
1
-1
*/
#include<bits/stdc++.h>
using namespace std;
int main(){
    int n,q,put;
    cin>>n>>q;
    unordered_map<int,int> um;
    for(int i=0;i<n;i++) {
        cin>>put;
        um[put]++;
    }
    for(int i=0;i<q;i++) {
        cin>>put;
        if (um.count(put)) cout<<um[put]<<endl;
        else cout<<-1<<endl;
    }
    system("pause");
    return 0;
}