class Solution_3
{
public:
	vector<int> bucket;
	int target = 0, sum;
	bool dfs(int index, vector<int>& nums)
	{
		if (index == nums.size())	return true;

		for (int i = 0; i < 4; i++) {
			if (bucket[i] + nums[index] > target || (i > 0 && bucket[i] == bucket[i - 1]))
				continue;
			bucket[i] += nums[index];
			if (dfs(index + 1, nums))	return true;
			bucket[i] -= nums[index];
		}
		return false;
	}
	bool is_square(vector<int>& nums)
	{
		bucket = vector<int>(4);
		sort(nums.begin(), nums.end());
		sum = accumulate(nums.begin(), nums.end(), 0);
        target = sum / 4;
        if (sum % 4) return 0;
		return dfs(0, nums);
	}
};