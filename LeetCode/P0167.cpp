class Solution {
public:
    vector<int> twoSum(vector<int>& numbers, int target) {
        vector<int> ans(2);
        int l=0,r=numbers.size()-1,mid;
        while (l<r)
        {
            mid=l+(r-l)/2;
            if (numbers[l]+numbers[mid]>target) {r=mid;continue;}
            else if (numbers[r]+numbers[mid]<target) {l=mid+1;continue;}
            else if (numbers[l]+numbers[r]>target) r--;
            else if (numbers[l]+numbers[r]<target) l++;
            else return{l+1,r+1};
        }
        return {-1,-1};
    }
};