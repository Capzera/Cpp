#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, x;
    unordered_map<int, int> mp;
    cin >> n;
    for (int i = 1; i <= n; i++) {
        cin >> x;
        mp[x] = i;
    }
    cin >> n;
    while (n--) {
        cin >> x;
        cout << mp[x] << endl;
    }
    system("pause");
    return 0;
}