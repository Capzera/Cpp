#include<bits/stdc++.h>
using namespace std;
int main(){
    string s;
    int n,a[252];
    cin>>s;
    scanf("%d",&n);
    int len=s.length(),l=0;
    for (int i=0;i<len;i++) a[i+1]=s[i]-'0';
    a[len+1]=0;
    for (int k=1;k<=n;k++)
        for(int i=1;i<=len;i++){
            if (a[i]>a[i+1]){
                for(int j=i;j<len;j++) a[j]=a[j+1];
                len--;
                a[len+1]=0;
                break;
            }
        }
    int i=1;
    while (a[i]==0 && l<len-1){
        l++;i++;
    }
    for(int i=l+1;i<=len;i++) printf("%d",a[i]);
    system("pause");
    return 0;
}