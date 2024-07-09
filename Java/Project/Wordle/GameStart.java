import javax.swing.*;

public class GameStart {

    public static void main(String[] args) {

        JFrame frame=new JFrame("WORDLE");

        frame.setBounds(300,100,1200,800);
        frame.setResizable(false);
        frame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);

        frame.add(new GamePanel());
        frame.setVisible(true);

    }
}