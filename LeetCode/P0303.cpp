class NumArray {
public:
    vector<int> sum;
    NumArray(vector<int>& nums) {
        int i;
        sum.resize(nums.size()+1);
        for(i=0;i<nums.size();i++) sum[i+1]=sum[i]+nums[i];
    }
    int sumRange(int left, int right) {
       return  sum[right+1]-sum[left];
    }
};

/**
 * Your NumArray object will be instantiated and called as such:
 * NumArray* obj = new NumArray(nums);
 * int param_1 = obj->sumRange(left,right);
 */