#include<bits/stdc++.h>
using namespace std;
int main(){
    int n,k;
    long long mid,left,right=0;
    int a[100001];
    cin>>n>>k;
    a[0]=100000001;
    for(int i=1;i<=n;i++){
        cin>>a[i];
        if(a[i]<a[0])a[0]=a[i];
        right+=a[i];
    }
    left=a[0];
    while (right-left>1){
        int tot=0,cot=1;
        mid=(left+right)/2;
        for(int i=1;i<=n;i++){
            if (tot+a[i]<=mid) {
                tot+=a[i];
            }
            else {tot=a[i];cot++;}
        }
        if (cot>k) left=mid;
        else right=mid;
    }
    cout<<left;
    system("pause");
    return 0;
}