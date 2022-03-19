/*
题目描述：
酋长同学来自遥远的埃及，这一天法老王交给了酋长同学一个建造金字塔得到任务：法老王给了酋长同学n个砖块，
并给了他这n个砖块的样子（用单个字符来表示），现在要他用这n个砖块造出可以建造的最大金字塔，并汇报剩余砖块的数量。
例如：当砖块数量为10个，砖块样子为'*'，则可以建造如下最大金字塔，且剩余1个砖块
输入
输入数据只有一行，包括一个整数 n 和一个字符 c，n表示酋长同学当前拥有的砖块数量，c表示这个砖块的样子。（n  <= 1000）
输出
首先用所给字符, 打印出这个最大金字塔的形状,
接下来一行输出剩余砖块的数量
输入样例 1 
10 *
输出样例 1
  *
 ***
*****
1
输入样例 2 
1 #
输出样例 2
#
0
*/
#include<bits/stdc++.h>
using namespace std;
int main(){
    int n,wide=1,maxdepth=0;
    char origin;
    cin>>n>>origin;
    while (1) {
        if (n-wide>=0) {
            maxdepth++;
            n-=wide;
            wide+=2;
        }
        else break;
    }
    for(int i=0;i<maxdepth;i++) {
        for(int j=0;j<maxdepth-i-1;j++) cout<<" ";
        for(int j=0;j<2*i+1;j++) cout<<origin;
        cout<<endl;
    }
    cout<<n<<endl;
    system("pause");
	return 0; 
}