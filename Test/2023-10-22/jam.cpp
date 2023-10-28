#include <bits/stdc++.h>
using namespace std;
int main() {
    int s, t, w;
    string jam;
    cin >> s >> t >> w >> jam;
    for (int i = 1; i <= 5; i++) {
        for (int j = w - 1; j >= 0; j--) {
            if (jam[j] + 1 <= 'a' + t - (w - j)) {
                jam[j]++;
                for(int k = j + 1; k < w; k++) {
                    jam[k] = jam[k - 1] + 1;
                }
                cout << jam << endl;
                break;
            }
        }
    }
    system("pause");
    return 0;
}
/*
s t w
2 10 5
bdfij
0 1 2 3 4
6 7 8 9 10
2 4 6 9 10
2 4 7 8 9
2 4 7 8 10
bcdefghij

bd
'a' + t - (w - j);
*/