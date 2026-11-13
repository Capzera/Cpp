class Solution {
public:
    vector<vector<int>> matrixReshape(vector<vector<int>>& mat, int r, int c) {
        if (r*c!=mat.size()*mat[0].size()) return mat;
        vector<vector<int>> m(r,vector<int>(c));
        int i,j,x=0,y=0;
        for(i=0;i<mat.size();i++)
            for(j=0;j<mat[0].size();j++)
            {
                m[x][y]=mat[i][j];
                y++;
                if (y==c)
                {
                    y=0;
                    x+=1;
                }
            }
        return m;
    }
};