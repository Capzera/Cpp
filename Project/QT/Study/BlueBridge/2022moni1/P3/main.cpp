#include <bits/stdc++.h>
using namespace std;
int main(){
    double sum = 0, i = 1;
    for (; ; i++) {
        double tmp = 1 / (double)i;
        sum += tmp;
        if (sum > 12) break;
    }
    cout << i << endl;
    return 0;
}
