/*
问题描述：
问题描述
　　123321是一个非常特殊的数，它从左边读和从右边读是一样的。
　　输入一个正整数n， 编程求所有这样的五位和六位十进制数，满足各位数字之和等于n 。
输入格式
　　输入一行，包含一个正整数n。
输出格式
　　按从小到大的顺序输出满足条件的整数，每个整数占一行。
样例输入
52
样例输出
899998
989989
998899
*/
#include<bits/stdc++.h>
using namespace std;
int main()
{
    int sum,i;
    cin>>sum;
    vector<int> vessel(3);
    vessel.clear();
    if (sum<=45&&sum>1)
    {
        if (sum<45)
            for(i=100;i<=999;i++)
            {
                vessel.push_back(i%10);
                vessel.push_back(i/10%10);
                vessel.push_back(i/100);
                //cout<<vessel[2]<<" "<<vessel[1]<<" "<<vessel[0]<<endl;
                if (vessel[0]+2*vessel[1]+2*vessel[2]==sum) 
                {
                    string ans="";
                    ans+=vessel[2]+48;
                    ans+=vessel[1]+48;
                    ans+=vessel[0]+48;
                    ans+=vessel[1]+48;
                    ans+=vessel[2]+48;
                    cout<<ans<<endl;
                }
                vessel.clear();
            }
        else cout<<99999<<endl;
    }
    if (sum%2==0)
    {
        for(i=100;i<=999;i++)
        {
            vessel.push_back(i%10);
            vessel.push_back(i/10%10);
            vessel.push_back(i/100);
            if (2*vessel[0]+2*vessel[1]+2*vessel[2]==sum) 
            {
                string ans="";
                ans+=vessel[2]+48;
                ans+=vessel[1]+48;
                ans+=vessel[0]+48;
                ans+=vessel[0]+48;
                ans+=vessel[1]+48;
                ans+=vessel[2]+48;
                cout<<ans<<endl;
            }
            vessel.clear();
        }
    }
    system("pause");
    return 0;
}