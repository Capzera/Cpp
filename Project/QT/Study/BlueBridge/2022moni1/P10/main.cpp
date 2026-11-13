#include <bits/stdc++.h>
#define ll long long
using namespace std;
int main(){
    cin.tie(nullptr)->sync_with_stdio(false);
    string s, anss = "";
    cin >> s;
    int n = s.size(), ans = -1;
    char minc = 91;
    vector<int> nums(n), h;
    for (int i = 0; i < n; i++) {
        nums[i] = (s[i] - 64 + i) % n;
        if (s[i] <= minc) {
            if (s[i] < minc) h.clear();
            minc = s[i];
            h.push_back(i);
        }
    }
    for (int i = 0; i < h.size(); i++) {
        vector<bool> vis(n);
        int j = h[i], k = 0;
        string tmp = "";
        bool flag = 1;
        while (!vis[j]) {
            vis[j] = 1;
            tmp += s[j];
            if (anss != "" && k < anss.size() && tmp[k] > anss[k]) {
                flag = 0;
                break;
            }
            k++;
            j = nums[j];
        }
        if (flag && anss != tmp) {
            anss = tmp;
            ans = h[i];
        }
    }
    cout << ans << endl;
    return 0;
}
