#include<bits/stdc++.h>
using namespace std;
string HAAPlus(string s1,string s2)
{
    int a[10001],b[10001];
    int len1=s1.size(),len2=s2.size();
    int i,cache=0,count;
    string ans="";
    memset(a,0,sizeof(a));
    memset(b,0,sizeof(b));
    reverse(s1.begin(),s1.end());
    reverse(s2.begin(),s2.end());
    for(i=1;i<=len1;i++) a[i]=s1[i-1]-'0';
    for(i=1;i<=len2;i++) b[i]=s2[i-1]-'0';
    for(i=1;i<=max(len1,len2);i++)
    {
        count=a[i]+b[i]+cache;
        if (count<=9)
        {
            cache=0;
            ans+=count+'0';
        }
        else
        {
            cache=1;
            ans+=count+'0';
        }
    }
    if (cache) ans+='1';
    reverse(ans.begin(),ans.end());
    return ans;
}
int main(){
    string s1,s2;
    cin>>s1;
    cin>>s2;
    cout<<HAAPlus(s1,s2)<<endl;
    system("pause");
    return 0;
}