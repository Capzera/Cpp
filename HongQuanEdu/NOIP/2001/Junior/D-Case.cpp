/*
有一个箱子容量为 V，同时有 n 个物品，每个物品有一个体积。

现在从 n 个物品中，任取若干个装入箱内（也可以不取），使箱子的剩余空间最小。输出这个最小值。
*/
#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, v, w;
    cin >> v >> n;
    vector<int> dp(v + 1);
    while (n--) {
        cin >> w;
        for (int j = v; j >= w; j--)
            dp[j] = max(dp[j], dp[j - w] + w);
    }
    cout << v - dp[v] << endl;
    system("pause");
    return 0;
}