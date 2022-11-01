/*
问题描述：P0037
编写一个程序，通过填充空格来解决数独问题。
数独的解法需 遵循如下规则：
数字?1-9?在每一行只能出现一次。
数字?1-9?在每一列只能出现一次。
数字?1-9?在每一个以粗实线分隔的?3x3?宫内只能出现一次。（请参考示例图）
数独部分空格内已填入了数字，空白格用?'.'?表示。
示例：
输入：board = 
[
["5","3",".",".","7",".",".",".","."],
["6",".",".","1","9","5",".",".","."],
[".","9","8",".",".",".",".","6","."],
["8",".",".",".","6",".",".",".","3"],
["4",".",".","8",".","3",".",".","1"],
["7",".",".",".","2",".",".",".","6"],
[".","6",".",".",".",".","2","8","."],
[".",".",".","4","1","9",".",".","5"],
[".",".",".",".","8",".",".","7","9"]
]
输出：
[
["5","3","4","6","7","8","9","1","2"],
["6","7","2","1","9","5","3","4","8"],
["1","9","8","3","4","2","5","6","7"],
["8","5","9","7","6","1","4","2","3"],
["4","2","6","8","5","3","7","9","1"],
["7","1","3","9","2","4","8","5","6"],
["9","6","1","5","3","7","2","8","4"],
["2","8","7","4","1","9","6","3","5"],
["3","4","5","2","8","6","1","7","9"]]
解释：输入的数独如上图所示，唯一有效的解决方案如下所示：
提示：
board.length == 9
board[i].length == 9
board[i][j] 是一位数字或者 '.'
题目数据 保证 输入数独仅有一个解
*/
class Solution {
public:
    bool row[9][9],column[9][9],grid[3][3][9];
    vector<pair<int,int>> space;
    int n;
    bool check=false;
    void dfs(vector<vector<char>>& board,int pos)
    {
        if (pos==n)//填完最后一个空格
        {
            check=1;
            return;
        }
        int i=space[pos].first,j=space[pos].second;
        for(int digit=0;digit<9&&!check;digit++)
        {
            if (row[i][digit]||column[j][digit]||grid[i/3][j/3][digit]) continue;//待填数不可用，跳过
            row[i][digit]=column[j][digit]=grid[i/3][j/3][digit]=1;//做标记
            board[i][j]=digit+49;//填数
            dfs(board,pos+1);//继续寻找下一个空格
            row[i][digit]=column[j][digit]=grid[i/3][j/3][digit]=0;//回溯
        }
    }
    void init(vector<vector<char>>& board)
    {
        memset(row,0,sizeof(bool));
        memset(column,0,sizeof(bool));
        memset(grid,0,sizeof(bool));
        for(int i=0;i<9;i++)
        {
            for(int j=0;j<9;j++)
            {
                if (board[i][j]=='.') space.push_back({i,j});
                else
                {
                    row[i][board[i][j]-49]=1;
                    column[j][board[i][j]-49]=1;
                    grid[i/3][j/3][board[i][j]-49]=1;
                }
            }
        }
        this->n=space.size();
    }
    void solveSudoku(vector<vector<char>>& board) {
        init(board);
        dfs(board,0);
    }
};