class Solution {
public:
    int maxPower(string s) {
        int i,cot=1,ans=1,len=s.length();
        for(i=1;i<len;i++)
        {
            if (s[i]!=s[i-1])
            {
                ans=max(ans,cot);
                if(len-i<=ans) break;
                cot=1;
            }
            else cot++;
        }
        return max(cot,ans);
    }
};