#include <bits/stdc++.h>
using namespace std;
int main() {
    vector<int> missile, pipe, cnt;
    string line;
    getline(cin, line);
    stringstream sin(line);
    int val;
    while (sin >> val) missile.emplace_back(val);
    for (auto x : missile) {
        if (pipe.empty() || x <= pipe.back()) pipe.emplace_back(x);
        else *upper_bound(pipe.begin(), pipe.end(), x, greater<int>()) = x;
        cout << pipe.size() << endl;
        if (cnt.empty() || x > cnt.back()) cnt.emplace_back(x);
        else *lower_bound(cnt.begin(), cnt.end(), x) = x;
    }
    cout << pipe.size() << endl << cnt.size() << endl;
    system("pause");
    return 0;
}