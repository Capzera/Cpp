import javax.swing.*;

public class GameStart {

    public static void main(String[] args) {

        //设置绘制面板
        JFrame frame=new JFrame("WORDLE");

        //面板基础属性
        frame.setBounds(300,100,1200,800);
        frame.setResizable(false);
        frame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);

        frame.add(new GamePanel());
        frame.setVisible(true);

    }
}