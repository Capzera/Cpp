/*
问题描述：P0433
基因序列可以表示为一条由 8 个字符组成的字符串，其中每个字符都是 'A'、'C'、'G' 和 'T' 之一。

假设我们需要调查从基因序列?start 变为 end 所发生的基因变化。一次基因变化就意味着这个基因序列中的一个字符发生了变化。

例如，"AACCGGTT" --> "AACCGGTA" 就是一次基因变化。
另有一个基因库 bank 记录了所有有效的基因变化，只有基因库中的基因才是有效的基因序列。
给你两个基因序列 start 和 end ，以及一个基因库 bank ，
请你找出并返回能够使?start 变化为 end 所需的最少变化次数。如果无法完成此基因变化，返回 -1 。
注意：起始基因序列?start 默认是有效的，但是它并不一定会出现在基因库中。
示例 1：
输入：start = "AACCGGTT", end = "AACCGGTA", bank = ["AACCGGTA"]
输出：1
示例 2：
输入：start = "AACCGGTT", end = "AAACGGTA", bank = ["AACCGGTA","AACCGCTA","AAACGGTA"]
输出：2
示例 3：
输入：start = "AAAAACCC", end = "AACCCCCC", bank = ["AAAACCCC","AAACCCCC","AACCCCCC"]
输出：3
提示：
start.length == 8
end.length == 8
0 <= bank.length <= 10
bank[i].length == 8
start、end 和 bank[i] 仅由字符 ['A', 'C', 'G', 'T'] 组成
*/
class Solution {
public:
    string e;
    queue<string> q;
    set<string> banks;
    bool compare(string s1,string s2){
        int dif=0;
        for (int i=0;i<8;i++) {
            if (s1[i]!=s2[i]) dif++;
            if (dif>1) return false;
        }
        return dif==1;
    }
    int bfs(int depth) {
        int size=q.size();//保存当前BFS队列长度
        bool c=true;//记录是否可以进行下一层搜索
        while (size--) {
            string tmp=q.front();q.pop();//取出队头元素
            if (tmp==e) return depth;//如果找到end就返回层
            for (auto s:banks) {
                if (compare(tmp,s)) {//如果当前元素为队头元素的下一层搜索
                    q.push(s);//加入队尾
                    banks.erase(s);//移除该元素
                    c=false;//记录发生过修改
                }
            }
        }
        if (c) return -1;//如果未发生过修改，说明不可以进行下一层搜索，无法找到目标
        return bfs(depth+1);//BFS下一层
    }
    int minMutation(string start, string end, vector<string>& bank) {
        if (start==end) return 0;//特判起点终点相同时
        for (auto s:bank) banks.insert(s);//将bank元素加入集合banks
        e=end;//保存搜索终点
        q.push(start);//记录起点
        return bfs(0);
    }
};