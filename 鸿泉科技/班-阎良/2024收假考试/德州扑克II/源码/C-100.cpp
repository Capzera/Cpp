#include <iostream>
#include <string>
#include <vector>
using namespace std;
int main() {
    string s1, s2;
    cin >> s1 >> s2;
    int str = 0, flu = 0, fok = 0, tok = 0, op = 0, sum = 0;
    if (s2[0] == s2[1] && s2[0] == s2[2] && s2[0] == s2[3] && s2[0] == s2[4]) {
    	flu = 1;
	}
    vector<int> nums, cnt(15);
    for (int i = 0; i < s1.size(); i++) {
        if (s1[i] >= '2' && s1[i] <= '9') nums.push_back(s1[i] - '0');
        if (s1[i] == '1') {
            nums.push_back(10);
            i++;
        }
        if (s1[i] == 'J') nums.push_back(11);
        if (s1[i] == 'Q') nums.push_back(12);
        if (s1[i] == 'K') nums.push_back(13);
        if (s1[i] == 'A') nums.push_back(14);
        int num = nums.back();
        ++cnt[num];
        if (cnt[num] == 4) {
        	fok = 1;
		}
		if (cnt[num] == 3) {
			tok = 1;
		}
		if (cnt[num] == 2) {
			op = 1;
		}
    }
    for (int i = 1; i < 5; i++) {
        sum += nums[i] - nums[i - 1] == 1;
    }
    if (sum == 4) str = 1;
    if (str == 1 && flu == 1) cout << "Straight Flush";
    else if (fok == 1) cout << "Four-of-a-Kind";
    else if (flu == 1) cout << "Flush";
    else if (str == 1) cout << "Straight";
    else if (tok == 1) cout << "Three-of-a-Kind";
    else if (op == 1) cout << "One Pair";
    else cout << "High Card";
    return 0;
}
