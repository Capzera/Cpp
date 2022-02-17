/*
题目描述：
有一幅以二维整数数组表示的图画，每一个整数表示该图画的像素值大小，数值在 0 到 65535 之间。
给你一个坐标?(sr, sc)?表示图像渲染开始的像素值（行 ，列）和一个新的颜色值?newColor，让你重新上色这幅图像。
为了完成上色工作，从初始坐标开始，记录初始坐标的上下左右四个方向上像素值与初始坐标相同的相连像素点，
接着再记录这四个方向上符合条件的像素点与他们对应四个方向上像素值与初始坐标相同的相连像素点，……，
重复该过程。将所有有记录的像素点的颜色值改为新的颜色值。
最后返回经过上色渲染后的图像。
示例 1:
输入: 
image = [[1,1,1],[1,1,0],[1,0,1]]
sr = 1, sc = 1, newColor = 2
输出: [[2,2,2],[2,2,0],[2,0,1]]
解析: 
在图像的正中间，(坐标(sr,sc)=(1,1)),
在路径上所有符合条件的像素点的颜色都被更改成2。
注意，右下角的像素没有更改为2，
因为它不是在上下左右四个方向上与初始点相连的像素点。
*/
class Solution {
public:
    int move[4][2]={{-1,0},{0,-1},{0,1},{1,0}};//控制移动
    void dfs(vector<vector<bool>>& check,vector<vector<int>>& image,int x,int y,int newColor,int m,int n,int target)
    {
        if (check[x][y]==0) 
        {
            image[x][y]=newColor;//染色
            check[x][y]=1;//判断该格是否走过，防止死循环
        }
        else return;
        for(int i=0;i<4;i++) 
        {
            int dx=x+move[i][0],dy=y+move[i][1];
            if (dx>=0&&dx<m&&dy>=0&&dy<n&&image[dx][dy]==target)//判断该格是否可走
            {
                dfs(check,image,x+move[i][0],y+move[i][1],newColor,m,n,target);
            }
        }
    }
    vector<vector<int>> floodFill(vector<vector<int>>& image, int sr, int sc, int newColor) {
        int target=image[sr][sc],m=image.size(),n=image[0].size();
        vector<vector<bool>> check(m,vector<bool>(n));
        if (target==newColor) return image;
        dfs(check,image,sr,sc,newColor,m,n,target);
        return image;
    }
};