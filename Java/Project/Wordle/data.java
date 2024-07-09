import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.net.URL;
import java.util.ArrayList;
import java.util.Random;

import javax.swing.ImageIcon;

public class data {

    //获取单词
    public static String Answer;//获取抽取的单词
    public static char[] AnswerSplit;//Answer的数组形式
    public static ArrayList<String> Data=new ArrayList<String>();//容器类存储所有单词的数组

    public static int size;//查看存储个数

    //从文件中读取所需要的单词
    public String getWord() throws IOException {
        //获取指定文件
        String path="src\\dictionary.txt";
        BufferedReader input= new BufferedReader(new FileReader(path));

        //从文件中读取内容
        String temp;//临时存放取出的单词,存入数组
        while ((temp=input.readLine())!=null){
            Data.add(temp);
        }
        size= Data.size();//获得单词总数

        //随机挑选一个数字取出
        Random random=new Random();
        int num= random.nextInt(size-1);
        //关闭流
        input.close();
        //返回随机挑选的单词
        return Data.get(num);
    }

    //获取图片信息
    public static URL BlockUrl=data.class.getResource("static/block.png");
    public static URL BlockUrl1=data.class.getResource("static/block2.png");
    public static URL BlockUrl2=data.class.getResource("static/block3.png");
    public static URL BlockUrl3=data.class.getResource("static/block1.png");
    public static ImageIcon Block=new ImageIcon(BlockUrl);
    public static ImageIcon Block1=new ImageIcon(BlockUrl1);
    public static ImageIcon Block2=new ImageIcon(BlockUrl2);
    public static ImageIcon Block3=new ImageIcon(BlockUrl3);


}
