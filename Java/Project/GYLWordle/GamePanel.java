import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.AWTException;
import java.awt.Robot;
import javax.swing.*;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import javax.swing.JButton;
import javax.swing.JPanel;

public class GamePanel extends JPanel implements ActionListener , KeyListener {

    // 键盘参数
    Robot robot;
    //line1到line3分别表示键盘的第一排到第五排，在Java中，每一个键有自己的keycode值，
    //例如：0~9的keycode是48~57，A~Z是65~90；
    int[] line1 = {81,87,69,82,84,89,85,73,79,80,8}; // 按键的第一排
    int[] line2 = {10,65,83,68,70,71,72,74,75,76};	// 第二排
    int[] line3 = {16,90,88,67,86,66,78,77}; // 第三排

    Map<Integer, String> uncharMap = new HashMap<Integer, String>(); // 特殊字符

    //页面参数
    final int len=5;//单词长度
    final int times=6;//最大尝试次数
    String[] keys=new String[5];//存储每行答案的字符
    Boolean isFail=false;//游戏默认状态是非失败
    Boolean isSuccess=false;//游戏默认状态是非成功
    int[][] WordX=new int[times][len];//存储展示单词的X坐标
    int[][] WordY=new int[times][len];//存储展示单词的Y坐标
    int space=100;//每个显示字母之间的间距
    Color color=Color.white;//默认字体颜色是白色

    //键盘监听事件的参数
    int pos;//对应列位置
    int x=0;//记录第几行
    char[] result=new char[5];//存储对比结果

    //判断结果参数
    int t=0;//记录T出现的次数
    JDialog winWindow = new JDialog();//获胜后弹出的窗口
    JDialog failWindow = new JDialog();//失败后弹出的窗口
    JDialog wrongWindow = new JDialog();
    //构造器
    public GamePanel() {
        init();
        this.setFocusable(true);//获得焦点事件  只有焦点事件添加后 监听才会聚焦到该栏目
        this.addKeyListener(this);//获得键盘监听事件
    }

    //初始化方法
    public void init(){
        //初始化参数
        t=0;
        pos=0;
        x=0;
        //winWindow.setVisible(false);
        //failWindow.setVisible(false);
        //遍历每行五个单词的显示位置
        for (int i = 0; i < times; i++) {
            for (int j = 0; j <len; j++) {
                WordX[i][j]=400+j*70;WordY[i][j]=60+i*70;//第一个框（显示字母的位置）为400,60，每次向后移70间距
            }
        }
        
        // 键盘的初始化

        // 模拟输入
        try {
            robot = new Robot();
        } catch (AWTException e) {
            e.printStackTrace();
        }
        this.setLayout(null);
        int x = 200,y = 500 ,width = 60 , height = 40;

        // 替换特殊字符
        initUnChar();
        // 添加从 33 - 126 的ascii
        int[][] keyint = new int[3][];//定义一个三行的二维数组存储所有的键
        keyint[0] = line1;
        keyint[1] = line2;
        keyint[2] = line3;
        y = y + height + 20;//height=40;
        // load keys
        int startx = 0,cellspace = 7;
        loadKeys(line1,startx,cellspace,x, y, width, height);
        y = y + height + 20;

        // line2
        int[] tmpInt = new int[]{line2[0]}; // tab
        //第二行中，第一个键比较大，故而单独列出
        loadKeys(tmpInt,20,cellspace, x, y, width + width /2 , height);
        startx = 40 + width + width / 2 - cellspace * 2 ;
        tmpInt = new int[line2.length - 1];//新数组的长度为原数组长度减一
        System.arraycopy(line2, 1, tmpInt, 0, tmpInt.length);//给行中给每个键加入相应的字符
        loadKeys(tmpInt,startx ,cellspace, x, y, width, height);

        // line3
        y = y + height + 20;
        tmpInt = new int[]{line3[0]};
        loadKeys(tmpInt,50,cellspace, x, y, width * 2 , height);
        startx = 70 + width * 2 - cellspace * 2 ;
        tmpInt = new int[line3.length - 1];
        System.arraycopy(line3, 1, tmpInt, 0, tmpInt.length);
        loadKeys(tmpInt,startx,cellspace, x, y, width, height);


        //读取data内容初始化
        try {
            data.Answer = new data().getWord();//真正的正确答案
            data.AnswerSplit=data.Answer.toCharArray();//正确答案的数组形式
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
        System.out.println(data.Answer);

    }

    //画笔 游戏中的静态画面绘制地方
    @Override
    protected void paintComponent(Graphics g) {
        //继承父类有清屏效果不会卡顿
        super.paintComponent(g);
        //画标题
       g.setColor(Color.BLACK);
       g.setFont(new Font("TimesRoman",Font.BOLD,50));//设置字体大小
        g.drawString("Wordle",480,40);//设置具体内容
        //画方框
        for (int i = 0; i < times; i++) {
            for (int j = 0; j <len; j++) {
                data.Block.paintIcon(this, g, WordX[i][j], WordY[i][j]);//Block画到这个面板上，用g画笔，x画在.,y画在
            }
        }

    }

    public void RepaintComponent1(Graphics g,int row,int colum) {
        //画方框
                data.Block1.paintIcon(this, g, WordX[row][colum], WordY[row][colum]);//Block画到这个面板上，用g画笔，x画在.,y画在

        }
    public void RepaintComponent2(Graphics g,int row,int colum) {
        //画方框
        data.Block2.paintIcon(this, g, WordX[row][colum], WordY[row][colum]);//Block画到这个面板上，用g画笔，x画在.,y画在

    }
    public void RepaintComponent3(Graphics g,int row,int colum) {
        //画方框
        data.Block3.paintIcon(this, g, WordX[row][colum], WordY[row][colum]);//Block画到这个面板上，用g画笔，x画在.,y画在

    }


    //游戏中的动态画面绘制的地方
    public void paint(Graphics g ,String s,int row,int colum,Color color){
        //画字母展示区
        g.setFont(new Font("Book Antiqua Negreta",Font.BOLD,30));//设置字体大小
        g.setColor(color);//根据正确性确定颜色
        g.drawString(s,WordX[row][colum]+25, WordY[row][colum]+40);
    }
    public void Repaint(Graphics g ,String s,int row,int colum,Color color){
        //画字母展示区
        g.setFont(new Font("Book Antiqua Negreta",Font.BOLD,30));//设置字体大小
        g.setColor(color);//根据正确性确定颜色
        g.drawString(s,WordX[row][colum]+25, WordY[row][colum]+40);
    }

    //事件监听
    @Override
    public void actionPerformed(ActionEvent e) {
        // 定义键盘上的监听事件,模拟按键操作
        MyJButton jb = (MyJButton)e.getSource();
        int Key = jb.getValue();
        robot.keyPress(Key);
        robot.keyRelease(Key);

        if (t<5)
            t=0;
        //判断是否胜利
        if (t==5) {
            isSuccess = true;
            repaint();
            init();
        }
        //判断是否失败
        else if (x>=times) {
            isFail=true;//超过6次游戏失败
            repaint();
            init();
        }

        //如果失败了，弹窗提示
        if (isFail==true){
            failWindow.setVisible(true);
            failWindow.setBounds(750,400,300,300);
            Container container=failWindow.getContentPane();
            container.add(new JLabel("Sorry,you are wrong.\nThe true answer is "+data.Answer,SwingConstants.CENTER));
            isFail=false;
        }
        //如果成功了弹窗提示
        if (isSuccess==true){
            winWindow.setVisible(true);
            winWindow.setBounds(750,400,300,300);
            Container container=winWindow.getContentPane();
            container.add(new JLabel("Good job! \n You win this game.",SwingConstants.CENTER));
            isSuccess=false;
        }

    }
    // 初始化特殊字符，在键盘中加入特殊字符。
    public void initUnChar(){
        uncharMap.put(8, "<--");
        uncharMap.put(10, "Enter");
        uncharMap.put(16, "SHIFT");
    }

    //定义键盘监听事件
    @Override
    public void keyPressed(KeyEvent e) {
        //正常按键的时候
        int keyCode=e.getKeyCode();
            if (pos<5&&keyCode!=8&&keyCode!=10&&keyCode!=16&&isFail==false){//输入26个字母时的操作
                String temp;
                //将得到的按键字母存入keys,答案存入answer
                temp=String.valueOf((char) (keyCode+32));
                keys[pos]=temp;
                paint(this.getGraphics(),keys[pos],x,pos,color);//获取当前画笔    每次输入后绘制字母
                pos++;
            }else if (keyCode==10&&pos==5&&x<6&&isFail==false){//在输入回车确定答案的操作
                //比较答案是否正确
                char[] tempAnswer=new char[data.AnswerSplit.length];//将每次的正确答案赋值给临时答案
                for (int i = 0; i < data.AnswerSplit.length; i++) {//重点：不能直接用等号赋值，不然tempAnswer指向的是data.AnswerSplit的地址，而不是赋值
                    char temp=data.AnswerSplit[i];
                    tempAnswer[i]=temp;
                }
                String compare;//临时将char转化成String进行比较
               String demo=String.join("",keys);
                if(data.Data.contains(demo)){//第一对比相同
                    for (int i = 0; i < 5; i++) {
                        compare=String.valueOf(tempAnswer[i]);//如果纵向对比相等赋值为T
                        if(compare.equals(keys[i])){//第一对比相同
                            result[i]='T';
                            tempAnswer[i]=0;
                        }else {
                            result[i]='F';
                        }
                    }
                    //第二次对比
                    for (int i = 0; i < 5; i++) {
                        if (result[i]=='T')
                            continue;
                        else {
                            for (int j = 0; j < 5; j++) {
                                compare=String.valueOf(tempAnswer[j]);
                                if (keys[i].equals(compare)){//第二次对比相同
                                    tempAnswer[j]=0;
                                    result[i]='C';
                                }
                            }
                        }
                    }
                    for (int i = 0; i < len; i++) {
                        if (result[i]=='T')
                            t++;
                    }
                    for (int i = 0; i < len; i++) {
                        if (result[i]=='T')
                            //Repaint(this.getGraphics(),keys[i],x,i,Color.GREEN);//替换当前颜色
                            RepaintComponent1(this.getGraphics(),x,i);
                        paint(this.getGraphics(),keys[i],x,i,color);
                        if (result[i]=='C')
                            //Repaint(this.getGraphics(),keys[i],x,i,Color.YELLOW);//替换当前颜色
                            RepaintComponent2(this.getGraphics(),x,i);
                        paint(this.getGraphics(),keys[i],x,i,color);
                        if (result[i]=='F')
                            // Repaint(this.getGraphics(),keys[i],x,i,Color.RED);//替换当前颜色
                            RepaintComponent3(this.getGraphics(),x,i);
                        paint(this.getGraphics(),keys[i],x,i,color);
                    }


                    x++;//换行
                    pos=0;//归零
                    for (int i = 0; i < 5; i++) {//清空用户输入
                        keys[i]="";
                    }
                }else {
                    wrongWindow.setVisible(true);
                    wrongWindow.setBounds(750,400,350,300);
                    Container container=wrongWindow.getContentPane();
                    container.add(new JLabel("The characters you entered are invalid, \nplease try again",SwingConstants.CENTER));

                }

    }

        //输入Delete的时候
        if (keyCode==8&&pos!=0){
            pos=pos-1;
            Repaint(this.getGraphics(),keys[pos],x,pos,new Color(205,205,205));//删除后重新绘制
            keys[pos]="";
        }


    }
    @Override
    public void keyTyped(KeyEvent e) {

    }

    @Override
    public void keyReleased(KeyEvent e) {

    }

    class MyJButton extends JButton{             //定义一个内部JButton类
        int value;
        public MyJButton(String showStr) {
            super(showStr);
        }
        public void setValue(int value) {
            this.value = value;
        }
        public int getValue() {
            return value;
        }
    }

    // 加载键盘
    public void loadKeys(int[] line,int startx,int cell,int x,int y ,int width,int height){
        // line 1
        for(int j = 0;j < line.length; j++){
            String showStr = uncharMap.get(line[j]); // 显示的字符，利用showstr来获取每个键的字符，
            if(showStr == null){
                showStr = String.valueOf((char) line[j]);
            }
            MyJButton jb = new MyJButton(showStr);
            jb.setBounds(startx + x + (cell + width) * j  , y, width, height);
            jb.setFocusable(false); //   最关键的一句话
            jb.setValue(line[j]); //
            jb.addActionListener(this);
            this.add(jb);
        }
    }
}
