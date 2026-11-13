#include <bits/stdc++.h>
using namespace std;

long long solve(int n) {
    long long ans = 0;
    for (int i = 1; i < n; i++) {
        if (i % 3 && i % 5) continue;
        ans += i;
    }
    return ans;
}

int main() {
    int n;
    cin >> n;
    cout << solve(n) << endl;
    system("pause");
    return 0;
}