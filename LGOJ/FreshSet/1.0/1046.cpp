#include <bits/stdc++.h>
using namespace std;
int main() {
    vector<int> apple(10);
    int chair, ans = 0;
    for (auto& x : apple) cin >> x;
    cin >> chair; chair += 30;
    for (auto& x : apple) {
        ans += x <= chair;
    }
    cout << ans << endl;
    system("pause");
    return 0;
}