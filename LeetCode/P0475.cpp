/*
问题描述：P0475
冬季已经来临。?你的任务是设计一个有固定加热半径的供暖器向所有房屋供暖。
在加热器的加热半径范围内的每个房屋都可以获得供暖。
现在，给出位于一条水平线上的房屋?houses 和供暖器?heaters 的位置，请你找出并返回可以覆盖所有房屋的最小加热半径。
说明：所有供暖器都遵循你的半径标准，加热的半径也一样。
示例 1:
输入: houses = [1,2,3], heaters = [2]
输出: 1
解释: 仅在位置2上有一个供暖器。如果我们将加热半径设为1，那么所有房屋就都能得到供暖。
示例 2:
输入: houses = [1,2,3,4], heaters = [1,4]
输出: 1
解释: 在位置1, 4上有两个供暖器。我们需要将加热半径设为1，这样所有房屋就都能得到供暖。
示例 3：
输入：houses = [1,5], heaters = [2]
输出：3
提示：
1 <= houses.length, heaters.length <= 3 * 104
1 <= houses[i], heaters[i] <= 109
*/
class Solution {
public:
    int findRadius(vector<int>& houses, vector<int>& heaters) {
        sort(heaters.begin(),heaters.end());
        set<int>tmp(houses.begin(),houses.end());
        houses.assign(tmp.begin(),tmp.end());//对houses去重减少工作量
        int n=houses.size(),h=heaters.size();
        int l=heaters[0]-houses[0],r=max(houses[n-1],heaters[h-1]),ans=INT_MAX;
        while (l<r)
        {
            int mid=l+(r-l)/2,j=0;//取中值，j指针记录当前使用的加热器
            for(int i=0;i<n;i++)
            {
                while(j<h)
                {
                    if (abs(heaters[j]-houses[i])>mid) j++;//当前加热器无法满足
                    else break;
                }
            }
            if (j<h) //mid为可解，mid-r均可解
            {
                ans=min(ans,mid);
                r=mid;//往左二分
            }
            else l=mid+1;//往右二分
        }
        return ans;
    }
};