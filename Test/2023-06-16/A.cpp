#include <bits/stdc++.h>
using namespace std;
int main() {
    string s;
    for (int i = 1; i <= 2023; i++) {
        s += to_string(i);
    }
    int n = s.size();
    vector<int> p2, p0, p3;
    for (int i = 0; i < n; i++) {
        if (s[i] == '2') p2.push_back(i);
        if (s[i] == '0') p0.push_back(i);
        if (s[i] == '3') p3.push_back(i);
    }
    long long ans = 0, m = p3.size();
    for (int i = 0; i < p2.size() - 1; i++) {
        for (int j = 0; j < p0.size(); j++) {
            if (p0[j] < p2[i]) continue;
            for (int k = i + 1; k < p2.size(); k++) {
                if (p2[k] < p0[j]) continue;
                ans += m - (lower_bound(p3.begin(), p3.end(), p2[k]) - p3.begin());
            }
        }
    }
    cout << ans << endl;
    system("pause");
    return 0;
}