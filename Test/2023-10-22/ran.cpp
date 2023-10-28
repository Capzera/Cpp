#include <bits/stdc++.h>
using namespace std;
int main() {
    vector<int> mp(31);
    for (int i = 1; i <= 6; i++) {
        for (int j = 1; j <= 6; j++) {
            for (int k = 1; k <= 6; k++) {
                for (int l = 1; l <= 6; l++) {
                    for (int m = 1; m <= 6; m++) {
                        mp[i + j + k + l + m]++;
                    }

                }
            }
        }
    }
    for (int i = 1; i <= 30; i++) 
        cout << i << " = " << mp[i] << endl;
    system("pause");
    return 0;
}