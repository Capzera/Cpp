class Solution {
public:
    bool canConstruct(string ransomNote, string magazine) {
        int check[27],len1=ransomNote.size(),len2=magazine.size(),i;
        memset(check,0,sizeof(check));
        for(i=0;i<len2;i++) 
            check[magazine[i]-96]++;
        for(i=0;i<len1;i++)
        {
            if (check[ransomNote[i]-96]==0) return false;
            check[ransomNote[i]-96]--;
        }
        return true;
    }
};