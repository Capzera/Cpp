class Solution {
public:
    vector<int> plusOne(vector<int>& digits) {
        int i,n=digits.size();
        digits[n-1]++;
        for(i=n-1;i>=0;i--)
        {
            if (digits[i]<10) break;
            else 
            {
                digits[i]=0;
                if (i==0) 
                {
                    digits[0]++;
                    digits.push_back(0);
                }
                else digits[i-1]++;
            }
        }
        return digits;
    }
};