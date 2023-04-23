#include <bits/stdc++.h>
using namespace std;
int main() {
    int k;
    string str;
    char a, b;
    cin >> k >> str >> a >> b;
    int n = str.size();
    vector<int> sum(n + 1);
    for (int i = 0; i < n; i++) {
        sum[i + 1] = sum[i] + (str[i] == a);
    }
    long long ans = 0;
    for (int i = k; i <= n; i++) {
        if (str[i - 1] != b) continue;
        ans += sum[i - k + 1];
    }
    cout << ans << endl;
    system("pause");
    return 0;
}
/*
 abababdb
011223333
1 + 2 + 3
*/