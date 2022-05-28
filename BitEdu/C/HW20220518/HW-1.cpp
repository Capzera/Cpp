#include<bits/stdc++.h>

using namespace std;

int main(){
    int n;
    cin>>n;
    int arr[n];
    for (int i=0;i<n;i++) cin>>arr[i];
    int* p=arr;
    for (int i=0;i<n;i++) cout<<*(p+i)<<" ";
    system("pause");
    return 0;
}