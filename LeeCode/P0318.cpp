class Solution {
public:
    int maxProduct(vector<string>& words) {
    int ans=0,i,j,c,map[27],n=words.size();
    long long a[1005];
    memset(a,0,sizeof(a));
    for (i=0;i<n;i++)
    {
        memset(map,0,sizeof(map));
        for (char c : words[i]) if(map[c-'a'+1]==0) map[c-'a'+1]=1;
        for (int j=1;j<=26;j++)if(map[j]!=0)a[i]+=1<<(j-1);
    }
    for(i=0;i<n-1;i++)
    {
        for(j=i+1;j<n;j++)
        {
            if ((a[i]&a[j])>0) continue;
            c=words[i].length()*words[j].length();
            if (c>ans) ans=c;
        }
    }
    return ans;
    }
};