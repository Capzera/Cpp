class Solution {
public:
    string longestCommonPrefix(vector<string>& strs) {
        int n=strs.size(),i,j,minlen=2000001,curlen,temp=0;
        string ans="";
        for(i=0;i<n;i++) 
        {
            curlen=strs[i].length();
            minlen=min(curlen,minlen);
        }
        for(i=0;i<minlen;i++)
        {
            for(j=1;j<n;j++)if (strs[j][i]!=strs[j-1][i]) temp=1;
            if (temp==0) ans+=strs[0][i];
            else break;
        }
        return ans;
    }
};