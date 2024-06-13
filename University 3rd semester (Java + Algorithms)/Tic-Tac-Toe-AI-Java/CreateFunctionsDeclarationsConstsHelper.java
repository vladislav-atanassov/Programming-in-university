import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.GridLayout;
import java.awt.BorderLayout;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JLabel;
import javax.swing.SwingConstants;

public class CreateFunctionsDeclarationsConstsHelper
{
    protected static JPanel boardPanel = new JPanel();
    
    protected static final int BOARD_SIZE = 3;
    protected static final JButton[] array = new JButton[BOARD_SIZE * BOARD_SIZE];
    
    protected static final ImageIcon xImage = new ImageIcon("C:\\Users\\Vladislav Atanassov\\Documents\\Java Begining\\Tic-Tac-Toe-AI-Java\\Images used\\X_icon_No_Background.png");
    protected static final ImageIcon circleImage = new ImageIcon("C:\\Users\\Vladislav Atanassov\\Documents\\Java Begining\\Tic-Tac-Toe-AI-Java\\Images used\\circle_PNG49.png");
    protected static final ImageIcon gameFrameImageIcon = new ImageIcon("C:\\Users\\Vladislav Atanassov\\Documents\\Java Begining\\Tic-Tac-Toe-AI-Java\\Images used\\download.png");
    
    protected static JLabel scoreMeassageLabelPlayer1 = new JLabel(), scoreMeassageLabelPlayer2 = new JLabel();
    
    protected static JLabel gameMessageLabel = new JLabel();
    protected static final String startingGameMessage = "Let's Play!";
    
    protected static int currentTurn = 1;
    protected static int currentScorePlayer1 = 0, currentScorePlayer2 = 0;
    
    protected static JButton resetButton = new JButton();
    
    protected static final int DELAY_ONE_SECOND = 1000; // 1000 milliseconds = 1 second
    protected static final String[] board = new String[BOARD_SIZE * BOARD_SIZE]; // Console implementation of the logic for the AI to use
    
    // Empty space to initialize the board
    public static final String EMPTY_SPACE = " ";

    // Values for USER vs AI
    public static final String AI = "O";
    public static final String AI_USER = "X";

    // Values for USER vs USER
    public static final String USER_1 = "X";
    public static final String USER_2 = "O";

    public final static String GAME_MODES[] = {"USER vs AI Level 1", "USER vs AI Level 2", "USER vs USER" };
    public final static int USER_VS_AI_LEVEL_1 = 0;    
    public final static int USER_VS_AI_LEVEL_2 = 1;
    public final static int USER_VS_USER = 2;

    protected static void setStartingBoard()
    {
        for (int i = 0; i < board.length; i++) 
        {
            board[i] = EMPTY_SPACE;
        }
    }

    protected JFrame createFrame(String text, int width, int height, boolean setResizable, boolean setVisible)
    {
        JFrame frame = new JFrame(text);

        frame.setSize(width,height);
        frame.setResizable(setResizable);
        frame.setVisible(setVisible);

        return frame;
    }

    protected static JButton createButton(Color color, int width, int height) 
    {
        JButton button = new JButton();

        button.setBackground(color);
        button.setPreferredSize(new Dimension(width, height));

        return button;
    }

    protected static JButton createButton(String text, Color color, int width, int height, int sizeFont, boolean setFocusPainted) 
    {
        JButton button = new JButton();

        button.setLayout(new BorderLayout());
        button.setPreferredSize(new Dimension(width, height));

        button.setText(text);
        button.setFont(new Font("Arial", Font.PLAIN, sizeFont));
        button.setForeground(Color.BLACK);

        button.setBackground(color);
        button.setFocusPainted(setFocusPainted);

        return button;
    }

    protected static JPanel createPanelGridLayout(int rows, int cols, int hgap, int vgap, Color color, int width, int height)
    {
        JPanel panel = new JPanel(new GridLayout(rows, cols, hgap, vgap));

        panel.setPreferredSize(new Dimension(width, height));
        panel.setBackground(color); 

        return panel;
    }

    protected static JPanel createPanelGridLayout(int rows, int cols, Color color, int width, int height)
    {
        JPanel panel = new JPanel(new GridLayout(rows, cols));

        panel.setPreferredSize(new Dimension(width, height));
        panel.setBackground(color); 

        return panel;
    }

    protected static JPanel createPanelFlowLayout(int align, int hgap, int vgap, Color color, int width, int height)
    {
        JPanel panel = new JPanel(new FlowLayout(align, hgap, vgap));

        panel.setPreferredSize(new Dimension(width, height));
        panel.setBackground(color); 

        return panel;
    }

    protected static JLabel createLabel(String text, int sizeFont)
    {
        JLabel label = new JLabel(text);

        label.setFont(new Font("Arial", Font.PLAIN, sizeFont));

        return label;
    }
}
