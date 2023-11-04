#include <bits/stdc++.h>
using namespace std;
int main() {
    double H, S, V, L, K, n;
    cin >> H >> S >> V >> L >> K >> n;
    double t1 = sqrt((H - K) / 5), t2 = sqrt(H / 5); 
    int s1 = S - V * t1 + L, s2 = S - V * t2;
    s1 = fmin(s1, n), s2 = fmax(s2, 0);
    cout << s1 - s2 << endl;
    system("pause");
    return 0;
}