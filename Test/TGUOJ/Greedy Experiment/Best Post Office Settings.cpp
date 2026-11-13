#include<bits/stdc++.h>
using namespace std;
int main(){
    int n,j=0;
    cin>>n;
    vector<int> H(n),P(1);
    for (int i=0;i<n;i++) cin>>H[i];
    P[0]=H[0]+1000;
    for (int i=1;i<n;i++) {
        if (H[i]>P[j]+1000) {
            j++;
            P.push_back(H[i]+1000);
        }

    }
    if (P[j]>H[n-1]) P[j]=H[n-1];
    cout<<"Total Post Office is:"<<j+1<<endl;
    for (int i:P) cout<<i<<" ";
    cout<<endl;
    system("pause");
    return 0;
}