#include <bits/stdc++.h>
using namespace std;
int n;
int dis(int a, int b) {
    return (a - b + 10) % 10;
}
bool check(vector<vector<int>>& code ,vector<int>& num) {
    for (int i = 0; i < n; i++) {
        if (num == code[i]) return 0;
        bool flag = 0;
        for (int j = 0; j < 5; j++) {
            if (code[i][j] != num[j]) {
                if (flag) return 0;
                if (dis(num[j], code[i][j]) == dis(num[j + 1], code[i][j + 1])) j++;
                flag = 1;
            }
        }
    }
    return 1;
}
int main() {
    cin >> n;
    int ans = 0;
    vector<vector<int> > code(n, vector<int>(6));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < 5; j++) cin >> code[i][j];
    for (int mask = 0; mask < 100000; mask++) {
        vector<int> num(6);
        int j = mask;
        for (int i = 4; i >= 0; i--) {
            num[i] = j % 10;
            j /= 10;
        }
        ans += check(code, num);
    }
    cout << ans << endl;
    system("pause");
    return 0;
}
/*
2 8 3 5 5
2 8 3 5 1

2 8 3 5 [2 3 4 6 7 8 9 0] + 

2 8 3 1 1
2 8 3 9 5
*/