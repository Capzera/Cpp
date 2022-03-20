/*
����������P6028 ���۵�285������
��һ�����޳��Ĺ�·���� n ������������ʻ�������������ҵ�˳�򰴴� 0 �� n - 1 ��ţ�ÿ��������һ�� ���ص� λ�á�
����һ���±�� 0 ��ʼ���ַ��� directions ������Ϊ n ��
directions[i] ������ 'L'��'R' �� 'S' �ֱ��ʾ�� i �������� �� ���� �� ���� ͣ�� �ڵ�ǰλ�á�ÿ�����ƶ�ʱ �ٶ���ͬ ��
��ײ�������԰�������ʽ���㣺
�������ƶ����� �෴ �ĳ���ײʱ����ײ������ 2 ��
��һ���ƶ��ĳ���һ����ֹ�ĳ���ײʱ����ײ������ 1 ��
��ײ�������漰�ĳ������޷������ƶ���ͣ������ײλ�á�����֮�⣬�������ܸı����ǵ�״̬���ƶ�����
������������·�Ϸ����� ��ײ�ܴ��� ��
ʾ�� 1��
���룺directions = "RLRSLL"
�����5
���ͣ�
�����ڵ�·�Ϸ�������ײ�г����£�
- �� 0 �ͳ� 1 �ụ����ײ���������ǰ��෴�����ƶ�����ײ������Ϊ 0 + 2 = 2 ��
- �� 2 �ͳ� 3 �ụ����ײ������ 3 �Ǿ�ֹ�ģ���ײ������Ϊ 2 + 1 = 3 ��
- �� 3 �ͳ� 4 �ụ����ײ������ 3 �Ǿ�ֹ�ģ���ײ������Ϊ 3 + 1 = 4 ��
- �� 4 �ͳ� 5 �ụ����ײ���ڳ� 4 �ͳ� 3 ��ײ֮�󣬳� 4 �������ײλ�ã����źͳ� 5 ��ײ����ײ������Ϊ 4 + 1 = 5 ��
��ˣ������ڵ�·�Ϸ�������ײ�ܴ����� 5 ��
ʾ�� 2��
���룺directions = "LLRR"
�����0
���ͣ�
�����ڻᷢ����ײ�ĳ�������ˣ������ڵ�·�Ϸ�������ײ�ܴ����� 0 ��
��ʾ��
1 <= directions.length <= 105
directions[i] ��ֵΪ 'L'��'R' �� 'S'
*/
class Solution {
public:
    int countCollisions(string directions) {
        int n=directions.size(),i=1,cnt=1;
        char s;
        long long ans=0;
        if (n<2) return 0;
        s=directions[0];
        while (i<n) {
            if (s=='R'&&directions[i]=='R') {
                cnt++;
            }
            else if (s=='R'&&directions[i]=='L') {
                s='S';
                ans+=2+cnt-1;
                cnt=0;
            }
            else if (s=='R'&&directions[i]=='S') {
                ans+=cnt;
                cnt=0;
            }
            else if (s=='S'&&directions[i]=='L') {
                ans++;
            }
            else if (s=='S'&&directions[i]=='R') {
                s='R';
                cnt=1;
            }
            else if (s=='L'&&directions[i]!='L') {
                s=directions[i];
            }

            i++;
        }
        return ans;
    }
};