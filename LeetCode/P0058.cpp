class Solution {
public:
    int lengthOfLastWord(string s) {
        int i=s.length()-1,ans=0,state=0;
        while((state==0)||(s[i]!=' '))
        {
            if (s[i]!=' ')
            {
                state=1;
                ans++;
            }
            i--;
            if (i<0) break;
        }
        return ans;
    }
};