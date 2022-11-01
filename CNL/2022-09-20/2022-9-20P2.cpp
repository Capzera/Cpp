#include <bits/stdc++.h>
using namespace std;
    int ans = INT_MAX, zerocnt, n, k;
    vector<int> package, bucket;
    void dfs(int index, int maxv){
        if (index == n){
            ans = min(ans, maxv);
            return;
        }
        for (int i = 0; i < k; i++){
            if (bucket[i] + package[index] > ans) continue;//如果当前桶中元素已经大于保存过的答案，剪枝1
            if (n - index < zerocnt) return;
            if (!bucket[i]) zerocnt--;
            bucket[i]  += package[index];
            dfs(index + 1, max(maxv, bucket[i]));
            bucket[i] -= package[index];
            if (!bucket[i]) zerocnt++;
        }
    }
int main() {
    cin >> n >> k;
    package.resize(n);
    bucket.resize(k);
    for (int i = 0; i < n; i++) cin>>package[i];
    bucket[0] = package[0];//把0号元素默认放在0号桶，因为k个桶与桶之间没有区别，剪枝2
    zerocnt = k - 1;
    dfs(1, package[0]);
    cout<<ans<<endl;
    system("pause");
    return 0;
}
/*
最大值最小
最小值最大
二分答案+dfs
*/