class Solution {
public:
    vector<int> intersect(vector<int>& nums1, vector<int>& nums2) {
        unordered_map<int,int> m; 
        int n1=nums1.size(),i,n2=nums2.size(),temp;
        if (n1>n2) return intersect(nums2,nums1);
        for(i=0;i<n1;i++) m[nums1[i]]++;
        vector<int> intersection;
        for(i=0;i<n2;i++) 
        {
            if (m.count(nums2[i]))
            {
                intersection.push_back(nums2[i]);
                m[nums2[i]]--;
            }               
            if (!m[nums2[i]]) m.erase(nums2[i]);
        }
        return intersection;
    }
};