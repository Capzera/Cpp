class Solution {
public:
    int strStr(string haystack, string needle) {
        int i=0,j=0,len1=haystack.length(),len2=needle.length();
        if (len2==0) return 0;
        while (i<len1)
        {
            if (haystack[i]==needle[0])
            {
                int cot=1,z=0,position=-1;
                for(j=1;j<len2;j++)
                {
                    if (haystack[i+j]==needle[0]&&z==0)
                    {
                        z=1;
                        position=i+j;
                    }
                    if (haystack[i+j]==needle[j]) cot++;
                    else break;
                }
                if (cot==len2) return i;
                else
                {
                    if (position!=-1) i=position-1;
                    else i+=j;
                }
            }
            i++;
        }
        return -1;
    }
};