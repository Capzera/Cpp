#include<bits/stdc++.h>
using namespace std;
const int r = 3, c = 3;
int squ[r][c];
int findM(int squ[][c]) {
    int mins = INT_MAX, maxs = INT_MIN;
    for(int i = 0; i < r; i++) {
        if(squ[i][0] < mins) mins = squ[i][0];
        if(squ[i][r - 1] > maxs) maxs = squ[i][r - 1];
    }
    while(mins < maxs) {
        int mid = mins + (maxs - mins) / 2;
        int count = 0;
        for(int i = 0; i < r; i++) {
            count += upper_bound(squ[i], squ[i] + c, mid) - squ[i];
        }
        if(count < (r * c + 1) / 2) mins = mid + 1;
        else maxs = mid;
    }
    return mins;
}
int main() {
    for(int i = 0; i < r; i++) 
        for(int j = 0; j < c; j++) 
            cin >> squ[i][j];
    int ans = findM(squ);
    cout << "矩阵的中位数是" << ans << endl;
    system("pause");
    return 0;
}
