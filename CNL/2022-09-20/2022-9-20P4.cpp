#include <bits/stdc++.h>

using namespace std;
    vector<int> nums;
    int n, L, k, ans = 0;
int main(){
    cin >> L >> n >> k;
    nums.resize(n);
    for (int i = 0; i < n; i++) cin >> nums[i];
    nums.push_back(L);
    int l = 0, r = L;
    while (l < r){
        int mid = l + (r - l) / 2;//作为本次猜测的答案
        int cnt = 0;//记录当前需要移走几颗石头
        int now = 0;//记录当前石头编号
        int i = 0;//下一块石头的位置
        while (i < n + 1){
            if (nums[++i] - nums[now] < mid) cnt++;
            else now = i;
            if (cnt > k) break;
        }
        if (cnt > k) r = mid;
        else {
            ans = mid;
            l = mid + 1;
        }
    }
    cout<<ans;
    system("pause");
    return 0;
}
/*
O(nlogL)
O(1)
*/