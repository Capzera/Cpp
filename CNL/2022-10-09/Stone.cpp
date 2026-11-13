#include <bits/stdc++.h>

using namespace std;

int main() {
    int L, D, N, ans;
    cin >> L >> D >> N;
    vector<int> nums(1);
    nums[0] = 0;
    for (int i = 0; i < N; i ++) {
        int put;
        cin>>put;
        nums.push_back(put);
    }
    nums.push_back(L);
    int l = 0, r = L;
    while (l < r) {
        int mid = l + (r - l) / 2, cnt = 0, last = 1;//猜最小的那个答案就是mid
        for (int i = 2; i <= n + 1; i++) {
            if (nums[i] - nums[last] < mid) cnt++;
            else last = i;
            if (cnt > D) break;
        }
        if (cnt > D) r = mid;
        else {
            ans = mid;
            l = mid + 1;
        }
    }
    cout << ans << endl;
    system("pause");
    return 0;
}
/*
    移走大于 D个石头 猜的太宽了，也就是mid太大了 R = mid;
    移走小于等于 D个石头 猜的太窄了 ，mid太小了，ans = mid;L = mid + 1;
*/