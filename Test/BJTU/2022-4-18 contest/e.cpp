#include<bits/stdc++.h>
using namespace std;
int n;
vector<int> nums,dp1,dp2,pos,neg,zero;
int main(){
    cin>>n;
    nums=dp1=dp2=pos=neg=zero=vector<int> (n);
    for (int i=0;i<n;i++) {
        cin>>nums[i];
        if (n) {
            pos[i]=pos[i-1];
            neg[i]=neg[i-1];
            zero[i]=zero[i-1];
        }
        if (nums[i]>0) pos[i]++;
        if (nums[i]==0) zero[i]++;
        if (nums[i]<0) neg[i]++;
    }
    if (nums[0]>0) dp1[0]=1;
    if (nums[0]<0) dp2[0]=1;
    for (int i=1;i<n;i++) {
        if (nums[i]>0) {
            dp1[i]++;
        }
        else if (nums[i]<0) {

        }
    }
    cout<<ans<<endl;
    return 0;
}
/*
5 -3 3 -1 0
-3
-1
-3 3
5 -3
5 -3 3
3 -1

5 -3 3 -1 0
-3 3 -1 0
3 -1 0
-1 0
0

5
5 -3 3 -1
-3 3 -1
3
pos 1 1 2 2 2
neg 0 1 1 2 0
zer 0 0 0 0 1

pos 1 1 2 4 4
neg 0 2 4 6 6
zer 0 0 0 0 5
*/ 