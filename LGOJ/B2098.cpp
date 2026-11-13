#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, x;
    unordered_set<int> mp;
    cin >> n;
    while (n--) {
        cin >> x;
        if (mp.count(x)) continue;
        cout << x << " ";
        mp.emplace(x);
    }
    system("pause");
    return 0;
}