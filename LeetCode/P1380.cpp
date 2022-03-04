class Solution {
public:
    vector<int> luckyNumbers (vector<vector<int>>& matrix) {
        int m=matrix.size(),n=matrix[0].size(),i,j;
        if (m==1&&n==1) return {matrix[0][0]};
        vector<int> row(m),ans;
        for(i=0;i<m;i++)
        {
            int MIN=INT_MAX;
            for(j=0;j<n;j++) MIN=min(MIN,matrix[i][j]);
            row[i]=MIN;
        }
        for(i=0;i<n;i++)
        {
            int MAX=INT_MIN,location=-1;
            for(j=0;j<m;j++)
            { 
                if (matrix[j][i]>MAX) 
                {
                    MAX=matrix[j][i];
                    location=j;
                }
            }
            if (MAX==row[location]) ans.push_back(MAX);
        }
        return ans;
    }
};