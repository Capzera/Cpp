#include <bits/stdc++.h>
using namespace std;
/*
题目描述
小苞准备开着车沿着公路自驾。
公路上一共有 n 个站点，编号为从 1 到 n。其中站点 i 与站点 i + 1 的距离为 v_i公里。
公路上每个站点都可以加油，编号为 i 的站点一升油的价格为 a_i元，且每个站点只出售整数升的油。
小苞想从站点 1 开车到站点 n，一开始小苞在站点 1 且车的油箱是空的。
已知车的油箱足够大，可以装下任意多的油，且每升油可以让车前进 d 公里。问小苞从站点 1 开到站点 n，至少要花多少钱加油？
*/
int main() {
    int n, d, mn = INT_MAX;
    cin >> n >> d; n--;
    int v[n], a[n];
    long long oil = 0, ans = 0;
    for (int i = 0; i < n; i++) cin >> v[i];
    for (int i = 0; i < n; i++) cin >> a[i];
    for (int i = 0; i < n; i++) {
        mn = min(mn, a[i]);
        int liter = (v[i] - oil + d - 1) / d;
        oil += 1ll * liter * d - v[i];
        ans += 1ll * liter * mn;
    }
    cout << ans << endl;
    system("pause");
    return 0;
}