#include <bits/stdc++.h>
using namespace std;
int main() {
    long long n, x, sum = 0;
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> x;
        sum ^= x;
    }
    cout << sum << " " << 0 << endl;
    system("pause");
    return 0;
}