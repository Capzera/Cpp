#include<bits/stdc++.h>
using namespace std;
string HAAMultiply(string s1,string s2)
{
    int a[10001],b[10001];
    int len1=s1.size(),len2=s2.size();
    int i,j,cache=0,count;
    string ans="0";
    memset(a,0,sizeof(a));
    memset(b,0,sizeof(b));
    if ((s1=="0")||(s2=="0")) return "0";
    reverse(s1.begin(),s1.end());
    reverse(s2.begin(),s2.end());           //翻转乘数
    for(i=0;i<len1;i++) a[i]=s1[i]-'0';
    for(i=0;i<len2;i++) b[i]=s2[i]-'0';  //将乘数转化为高精度数组
    for(i=0;i<len1;i++)
    {
        cache=0;
        for(j=0;j<len2;j++)
        {
            if (ans[i+j]==0) ans+='0';
            count=ans[i+j]-'0'+a[i]*b[j]+cache;
            cache=count/10;
            ans[i+j]=(count)%10+'0';
        }
        if (cache!=0) ans+=cache+'0';
    }
    reverse(ans.begin(),ans.end());
    return ans;
}
int main(){
    string s1,s2;
    cin>>s1;
    cin>>s2;
    cout<<HAAMultiply(s1,s2)<<endl;
    system("pause");
    return 0;
}