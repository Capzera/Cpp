#include <bits/stdc++.h>
using namespace std;
int n, c1, c2;
string a, b;
int main() {
    cin >> n >> a >> b;
    for (int i = 0; i < n; i++) {
        if (a[i] == '0' && b[i] == '1') c1++;
        if (a[i] == '1' && b[i] == '0') c2++;
    }
    cout << max(c1, c2) << endl;
    return 0;
}
