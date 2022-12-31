#include <bits/stdc++.h>
using namespace std;
int main() {  
    int n, k, preSum = 0, preFood = 0, index = -1;
    cin >> n >> k;
    vector<int> time(n), food(n);
    vector<bool> check(n);
    for (int i = 0; i < n; i++) cin >> time[i];
    for (int i = 0; i < n; i++) {
        cin >> food[i];
        if (food[i] * time[i] > preSum) {
            preSum = food[i] * time[i];
            preFood = food[i];
            index = i;
        }
    }
    check[index] = 1;
    while (--k) {
        index = -1;
        int contri = 0;
        for(int i = 0; i < n; i++) {
            if (check[i]) continue;
            int sum;
            if (food[i] >= preFood) sum = time[i] * preFood;
            else sum = (preSum / preFood + time[i]) * food[i] - preSum;
            if (sum > contri) {
                contri = sum;
                index = i;
            }
        }
        preSum += contri;
        check[index] = 1;
        preFood = min(preFood, food[index]);
    }
    cout << preSum << endl;
    system("pause");
    return 0;
}
/*
6 2
2 10 3 1 5 8
5 4 3 9 7 2

10 4
2 10 3 1 5 8 6 9 10 4
5 4 3 9 7 2 4 5 2 1
10 4
1 2 6 3 10 8 4
9 5 4 3 2  2 1
presum + time[i] * food[i];
(preSum / preFood + time[i]) * food[i] - preSum


preSum = 45, preFood = 9
5
val = 9
preSum = 70 preFood = 7
preSum = 80 preFood = 4
20
89 88 104 69 120 
print(preSum);
*/