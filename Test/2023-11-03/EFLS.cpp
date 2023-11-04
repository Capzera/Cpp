#include <bits/stdc++.h>
using namespace std;
int main() {
    vector<int> efls = {1, 2, 3, 4, 5};
    unordered_map<int, string> ans = {
        {1, "бл"},
        {2, "╨Л"},
        {3, "ю╤"},
        {4, "во"},
        {5, "╟в"}
    };
    set<vector<int> > mp;
    int i = 1;
    while (next_permutation(efls.begin(), efls.end())) {
        if (mp.count(efls)) continue;
        if (i < 10) cout << "0";
        cout << i++ << ": ";
        for (auto& x : efls) cout << ans[x] << " "; cout << endl;
        for (int i = 0; i < 5; i++) {
            int tmp = efls[0];
            for (int i = 0; i < 4; i++) efls[i] = efls[i + 1];
            efls.back() = tmp;
            mp.emplace(efls);
        }
    }
    system("pause");
}