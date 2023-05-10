#include<bits/stdc++.h>
using namespace std;
int main() {
    string s, tmp = "";
    cin >> s;
    vector<string> word, lis, ans;
    for (auto& ch : s){
        if (isupper(ch)) {
            if (tmp != "") word.push_back(tmp);
            tmp = ch;
        }
        else tmp += ch;
    }
    word.push_back(tmp);
    int n = word.size();
    vector<int> pos(n);
    for (int i = 0; i < n; i++) {
        if (lis.empty() || word[i] > lis.back()) {
            lis.push_back(word[i]);
        }
        pos[i] = lower_bound(lis.begin(), lis.end(), word[i]) - lis.begin();
        lis[pos[i]] = word[i];
    }
    for (int i = n - 1, j = lis.size() - 1; i >= 0; i--) {
        if (pos[i] == j) {
            ans.push_back(word[i]);
            j--;
        }
    }
    for (int i = ans.size() - 1; i >= 0; i--) cout << ans[i];
    cout << endl;
    return 0;
}