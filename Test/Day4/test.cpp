#include <bits/stdc++.h>
using namespace std;
int main() {
    int x = 9999, cnt = 0;
    while (x) {
        cnt ++;
        x &= x - 1;
    }
    cout << cnt << endl;
    system("pause");
    return 0;
}