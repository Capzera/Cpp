class Solution {
public:
    int maxNumberOfBalloons(string text) {
        vector<int> count(5);
        int i,ans=INT_MAX,got;
        unordered_map<char,int> mp{
            {'b',0},
            {'a',1},
            {'l',2},
            {'o',3},
            {'n',4}
        };
        for(i=0;i<text.size();i++) count[mp[text[i]]]++;
        count[2]/=2;count[3]/=2;
        for(i=0;i<5;i++) ans=min(count[i],ans);
        return ans; 
    }
};