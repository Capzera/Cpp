/*
��Ŀ������
    ����һ���ַ���?s?������Ҫ��ת�ַ�����ÿ�����ʵ��ַ�˳��ͬʱ�Ա����ո�͵��ʵĳ�ʼ˳��
ʾ�� 1��
����
    s = "Let's take LeetCode contest"
���
    "s'teL ekat edoCteeL tsetnoc"
ʾ�� 2:
���룺 
    s = "God Ding"
�����
    "doG gniD"
*/
class Solution {
public:
    string reverseWords(string s) {
        string ans="",temp="";
        int n=s.length(),i,op,ed,state=0;
        if (s.empty()||n==1) return s;
        s.push_back(' ');
        for(i=0;i<=n;i++)
        {
            if (s[i]==' ')
            {
                state=1;//state=1 ¼�뵥��
                reverse(temp.begin(),temp.end());//��ת¼��ĵ���
                ans+=temp;
                temp="";
                if (i!=n) ans+=' ';//�ų����һ���ո�¼��ո�
            }
            else if (state) state=0;//state=0¼��״̬
            if (!state) temp.push_back(s[i]);//��ǰΪ�����ڣ�ѹ�뵥��ջ
        }
        return ans;
    }
};