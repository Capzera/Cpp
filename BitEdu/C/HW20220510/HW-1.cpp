#include "bits/stdc++.h"

using namespace std;
vector<int> nums;
int n;

void bubbleSort() {
    int flag;
    for (int i=0;i<n;i++) {
        for (int j=n-1;j>i;j--) {
            if (nums[j]<=nums[j-1]) {
                swap(nums[j-1],nums[j]);
                flag=1;
            }
        }
        if (!flag) return;
    }
}

int main() {
    cin>>n;
    nums=vector<int> (n);
    for (int i=0;i<n;i++) cin>>nums[i];
    bubbleSort();
    for (auto i:nums) cout<<i<<" ";
    system("pause");
    return 0;
}