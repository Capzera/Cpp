/*
����������P0838
n �Ŷ���ŵ�����ų�һ�У���ÿ�Ŷ���ŵ���ƴ�ֱ�������ڿ�ʼʱ��ͬʱ��һЩ����ŵ��������������ơ�
ÿ��һ�룬������ߵĶ���ŵ���ƻ��ƶ���������ڵĶ���ŵ���ơ�ͬ���أ������ұߵĶ���ŵ����Ҳ���ƶ����������Ҳ�����ڶ���ŵ���ơ�
���һ�Ŵ�ֱ�����Ķ���ŵ���Ƶ�����ͬʱ�ж���ŵ���Ƶ���ʱ����������ƽ�⣬ �ù�����Ȼ���ֲ��䡣
�����������ԣ����ǻ���Ϊһ�����ڵ��µĶ���ŵ���Ʋ�����������ڵ��»��Ѿ����µĶ���ŵ����ʩ�Ӷ��������
����һ���ַ��� dominoes ��ʾ��һ�ж���ŵ���Ƶĳ�ʼ״̬�����У�
dominoes[i] = 'L'����ʾ�� i �Ŷ���ŵ���Ʊ�������࣬
dominoes[i] = 'R'����ʾ�� i �Ŷ���ŵ���Ʊ������Ҳ࣬
dominoes[i] = '.'����ʾû���ƶ��� i �Ŷ���ŵ���ơ�
���ر�ʾ����״̬���ַ�����
ʾ�� 1��
���룺dominoes = "RR.L"
�����"RR.L"
���ͣ���һ�Ŷ���ŵ����û�и��ڶ���ʩ�Ӷ��������
ʾ�� 2��
���룺dominoes = ".L.R...LR..L.."
�����"LL.RR.LLRRLL.."
��ʾ��
n == dominoes.length
1 <= n <= 105
dominoes[i] Ϊ 'L'��'R' �� '.'
*/
class Solution {
public:
    string pushDominoes(string dominoes) {
        int n=dominoes.size(),i=0;
        char l='L';//��ʼ��߽�����
        while (i<n)
        {
            int j=i;
            while (j<n&&dominoes[j]=='.') j++;//�ҵ������Ķ���ŵ����״̬
            char r=(j<n? dominoes[j]:'R');//�����һ��״̬Ϊ�߽磬�Զ���һ����״̬
            if (l==r)//����״̬��ͬ���м�Ĺ�����һ������
            {
                while (i<j) dominoes[i++]=r;
            }
            else if (l=='R'&&r=='L')//������ͬ�����ߵĹ������м䵹
            {
                int k=j-1;//�ұ߽�
                while (i<k)
                {
                    dominoes[i++]='R';//��߹�������
                    dominoes[k--]='L';//�ұ߹�������
                }
            }
            l=r;
            i=j+1;
        }
        return dominoes;
    }
};