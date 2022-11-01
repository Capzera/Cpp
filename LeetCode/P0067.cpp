class Solution {
public:
    string addBinary(string a, string b) {
    int i=a.size(),j=b.size(),n,temp=0,count;
    string s;
    char aa,bb;
    n=max(i--,j--);
    while (i>=0||j>=0)
    {
        if (i<0) aa='0';
        else aa=a[i];
        if(j<0) bb='0';
        else bb=b[j];
        count=aa+bb-'0'-'0'+temp;
        if (count>1)
        {
            s+=count%2+'0';
            temp=1;
        }
        else
        {
            s+=count+'0';
            temp=0;
        }
        i--;j--;
    }
    if (temp==1) s+='1';
    reverse(s.begin(),s.end());
    return s;
    }
};
