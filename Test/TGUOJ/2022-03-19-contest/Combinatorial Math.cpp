/*
题目描述：
龙龙学姐很喜欢学数学，而在数学问题里他最喜欢研究的就是排列组合问题了。想得到龙龙学姐的青睐就要通过他的考验！
现在他想考考你前n个正整数不同的排列组合方案有多少，请你写一个程序按字典序, 列出1~n所有的排列组合方法。
输入
输入数据只有一行，包括一个整数 n（1  <=  n  <= 7）
输出
按字典序输出1~n的所有可能的排列组合方案, 每个方案占一行
关于何为"字典序", 请参考下面的样例
注意每行行末需要额外输出一个空格(出题人懒得改数据了)
例如输出
1 2 3\n
是错的, 而
1 2 3 \n
才是对的
输入样例 1 
3
输出样例 1
1 2 3
1 3 2
2 1 3
2 3 1
3 1 2
3 2 1
*/
#include<bits/stdc++.h>
using namespace std;
    int n;
    void dfs(int i,vector<int> ans,vector<bool> check){
        if (i==n){
            for(int t:ans) cout<<t<<" ";
            cout<<endl;
            return;
        }
        for(int j=0;j<n;j++) {
            if (check[j]) continue;
            check[j]=1;
            ans[i]=j+1;
            dfs(i+1,ans,check);
            check[j]=0;
        }
    }
int main(){
    cin>>n;
    vector<int> ans(n);
    vector<bool>check(n);
    dfs(0,ans,check);
    system("pause");
    return 0;
}