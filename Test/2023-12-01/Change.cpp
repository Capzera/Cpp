#include <bits/stdc++.h>
using namespace std;

int solve(int gacha, int limit, int target, int initial) {
    return ((limit - gacha) * target - initial + gacha) / gacha;
}

int main() {
    int gacha = 20;
    int limit = 90;
    int target = 6;
    int initial = 30;
    cout << solve(gacha, limit, target, initial) << endl;
    system("pause");
    return 0;
}
/*

*/