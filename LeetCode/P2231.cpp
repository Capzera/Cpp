/*
问题描述：P2231 力扣第288场周赛
给你一个正整数 num 。你可以交换 num 中 奇偶性 相同的任意两位数字（即，都是奇数或者偶数）。
返回交换 任意 次之后 num 的 最大 可能值。
示例 1：
输入：num = 1234
输出：3412
解释：交换数字 3 和数字 1 ，结果得到 3214 。
交换数字 2 和数字 4 ，结果得到 3412 。
注意，可能存在其他交换序列，但是可以证明 3412 是最大可能值。
注意，不能交换数字 4 和数字 1 ，因为它们奇偶性不同。
示例 2：
输入：num = 65875
输出：87655
解释：交换数字 8 和数字 6 ，结果得到 85675 。
交换数字 5 和数字 7 ，结果得到 87655 。
注意，可能存在其他交换序列，但是可以证明 87655 是最大可能值。
提示：
1 <= num <= 109
*/
class Solution {
    static bool cmp(int a,int b){
        return a>b;
    }
public:
    int largestInteger(int num) {
        int ans=0;
        vector<int> nums;
        while (num) {
            int n=num%10;
            nums.push_back(n);
            num/=10;
        }
        reverse(nums.begin(),nums.end());
        int n=nums.size();
        vector<int> indexo,indexe;
        vector<int> odd,even;
        for (int i=0;i<n;i++) {
            if (nums[i]%2) {
                indexo.push_back(i);
                odd.push_back(nums[i]);
            }
            else{
                indexe.push_back(i);
                even.push_back(nums[i]);
            }
        }
        int x=0,y=0,i=0;
        sort(odd.begin(),odd.end(),cmp);
        sort(even.begin(),even.end(),cmp);
        while (i<n){
            if (x<odd.size()&&indexo[x]==i){
                ans=ans*10+odd[x++];
            }
            else if (y<even.size()&&indexe[y]==i) {
                ans=ans*10+even[y++];
            }
            i++;
        }
        return ans;
    }
};