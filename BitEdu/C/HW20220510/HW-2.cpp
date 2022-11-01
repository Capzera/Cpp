#include <bits/stdc++.h>

using namespace std;
int n;

void init(int *p){
    int m=n;
    while (m--){
        *p=0;
        p++;
    }
}

void print(int nums[]){
    for (int i=0;i<n;i++) cout<<nums[i]<<" ";
    cout<<endl;
}

void reverse(int* p){
    for (int i=1;i<n/2;i++) swap(*(p+i-1),*(p+n-i));

}

int main(){
    cin>>n;
    int nums[n];
    int *p=nums;
    init(nums);
    print(nums);
    for (int i=0;i<n;i++) cin>>nums[i];
    print(nums);
    cout<<*p<<endl;
    p=nums;
    reverse(p);
    print(nums);
    system("pause");
    return 0;
}