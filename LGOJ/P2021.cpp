#include <bits/stdc++.h>
using namespace std;
int main() {
    int n;
    cin >> n;
    deque<int> dq(n);
    vector<int> ans(n);
    iota(dq.begin(), dq.end(), 0);
    for (int i = 1; i <= n; i++) {
        dq.push_back(dq.front());
        dq.pop_front();
        ans[dq.front()] = i;
        dq.pop_front();
    }
    for (auto& x : ans) cout << x << " ";
    system("pause");
    return 0;
}