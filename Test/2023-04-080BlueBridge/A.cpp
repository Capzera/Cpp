#include <bits/stdc++.h>
using namespace std;
int moon[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
set<int> ans;
void check(int m, int a, int b, int c) {
    int mm = m * 10 + a, dd = b * 10 + c;
    int sum = mm * 100 + dd;
    if (ans.count(sum)) return;
    if (mm == 0 || mm > 12) return;
    if (dd == 0 || dd > moon[mm]) return;
    cout << sum << " ";
    ans.insert(sum);
}
void solve() {
    int n = 100, nums[100];
    vector<int> n0, n1, n2, n3;
    for (int i = 0; i < n; i++) {
        cin >> nums[i];
        if (nums[i] == 0) n0.push_back(i);
        if (nums[i] == 1) n1.push_back(i);
        if (nums[i] == 2) n2.push_back(i);
        if (nums[i] == 3) n3.push_back(i);
    }
    int l0 = n0.size(), l1 = n1.size(), l2 = n2.size(), l3 = n3.size();
    for (int p2 = 0, p0 = 0, p22 = 0, p3 = 0; p2 < l2 - 1; p2++) {
        while (p0 < l0 - 1 && n0[p0] < n2[p2]) p0++;
        while (p22 < l2 && n2[p22] < n0[p0]) p22++;
        while (p3 < l3 && n3[p3] < n2[p22]) p3++;
        int p00 = 0, p1 = 0;
        while (p00 < l0 && n0[p00] < n3[p3]) p00++;
        while (p1 < l1 && n1[p1] < n3[p3]) p1++;
        for (; p00 < l0; p00++)
            for (int i = n0[p00] + 1; i < 98; i++) {
                if (!nums[i]) continue;
                for (int j = i + 1; j < 99; j++) {
                    if (nums[j] > 3) continue;
                    for (int k = j + 1; k < 100; k++)
                        check(0, nums[i], nums[j], nums[k]);
                }
            }
        for (; p1 < l1; p1++)
            for (int i = n1[p1] + 1; i < 98; i++)
                for (int j = i + 1; j < 99; j++) {
                    if (nums[j] > 3) continue;
                    for (int k = j + 1; k < 100; k++)
                        check(1, nums[i], nums[j], nums[k]);
                }
    }
    cout << ans.size() << endl;
}
int main() {
    int t = 1;
    //cin >> t;
    while (t--) {
        solve();
    }
    system("pause");
    return 0;
}
/*
5 6 8 6 9 1 6 1 2 4 9 1 9 8 2 3 6 4 7 7 5 9 5 0 3 8 7 5 8 1 5 8 6
1 8 3 0 3 7 9 2 7 0 5 8 8 5 7 0 9 9 1 9 4 4 6 8 6 3 3 8 5 1 6 3 4
6 7 0 7 8 2 7 6 8 9 5 6 5 6 1 4 0 1 0 0 9 4 8 0 9 1 2 8 5 0 2 5 3 3
*/