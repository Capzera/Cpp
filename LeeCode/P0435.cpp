/*
    LeeCode P435 无重叠区间
    给定一个区间的集合，找到需要移除区间的最小数量，使剩余区间互不重叠。
    注意:
    可以认为区间的终点总是大于它的起点。
    区间 [1,2] 和 [2,3] 的边界相互“接触”，但没有相互重叠。
    算法：贪心 数组 动态规划 排序
*/
#include<bits/stdc++.h>
using namespace std;
int main(){
    int a[1001],b[1001];
    int i,j,len=0,n=0;
    while(a[n]!=0&&b[n]!=0){
        n++;
        cin>>a[n]>>b[n];
    }
    for(i=1;i<n;i++)
        for(j=i+1;j<=n;j++)
            if (b[i]>b[j])
            {
                swap(a[i],a[j]);
                swap(b[i],b[j]);
            }
    a[0]=b[1];
    for(i=2;i<=n;i++)
        if (a[i]<a[0]) len++;
            else a[0]=b[i];
    cout<<len<<endl;
    system("pause");
    return 0;
}
/*输入样例1：
1 2
2 3
3 4
1 3
0 0
输出样例1：
1
输入样例2：
1 2
1 2
1 2
0 0
输出样例2：
2
输入样例3：
1 2
2 3
0 0
输出样例3：
0
*/