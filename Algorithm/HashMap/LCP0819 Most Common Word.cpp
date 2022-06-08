/*
����������P0819
����һ������ (paragraph) ��һ�����õ����б� (banned)�����س��ִ�����࣬ͬʱ���ڽ����б��еĵ��ʡ�
��Ŀ��֤������һ���ʲ��ڽ����б��У����Ҵ�Ψһ��
�����б��еĵ�����Сд��ĸ��ʾ�����������š������еĵ��ʲ����ִ�Сд���𰸶���Сд��ĸ��
ʾ����
����: 
paragraph = "Bob hit a ball, the hit BALL flew far after it was hit."
banned = ["hit"]
���: "ball"
����: 
"hit" ������3�Σ�������һ�����õĵ��ʡ�
"ball" ������2�� (ͬʱû���������ʳ���2��)���������Ƕ�������ִ������ģ��Ҳ��ڽ����б��еĵ��ʡ� 
ע�⣬������Щ�����ڶ����ﲻ���ִ�Сд����������Ҫ���ԣ���ʹ�ǽ����ŵ���Ҳ���ԣ� ���� "ball,"���� 
"hit"�������յĴ𰸣���Ȼ�����ִ������࣬�����ڽ��õ����б��С�
��ʾ��
1 <= ���䳤�� <= 1000
0 <= ���õ��ʸ��� <= 100
1 <= ���õ��ʳ��� <= 10
����Ψһ��, �Ҷ���Сд��ĸ?(��ʹ�� paragraph ���Ǵ�д�ģ���ʹ��һЩ�ض������ʣ��𰸶���Сд�ġ�)
paragraph?ֻ������ĸ���ո�����б�����!?',;.
������û�����ַ����ߴ������ַ��ĵ��ʡ�
������ֻ������ĸ���������ʡ�ԺŻ������������š�
*/
class Solution {
    typedef pair<int,string> psi;
public:
    string mostCommonWord(string paragraph, vector<string>& banned) {
        for (auto& c:paragraph) if (c<91&&c>64) c+=32;
        unordered_map<string,int> map;
        set<string> s;
        string tmp="",ans="";
        int anscnt=0;
        for (auto& i:banned) s.insert(i);
        paragraph.push_back(' ');
        for (int i=0;i<paragraph.size();i++) {
            if (isalpha(paragraph[i])) tmp+=paragraph[i];
            else {
                if (tmp=="") continue;
                if (s.find(tmp)==s.end()) {
                    map[tmp]++;
                    if (map[tmp]>anscnt){
                        anscnt=map[tmp];
                        ans=tmp;
                    }
                }
                tmp="";
            }
        }
        return ans;
    }
};