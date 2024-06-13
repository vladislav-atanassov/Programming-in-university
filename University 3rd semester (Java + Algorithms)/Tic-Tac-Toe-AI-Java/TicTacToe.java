import java.awt.Color;
import java.awt.FlowLayout;
import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;

public class TicTacToe extends CreateFunctionsDeclarationsConstsHelper
{
    public TicTacToe() 
    {
        setStartingBoard();
        
        displayGameModeMenu(); //! UNIMPLEMENTED FULLY

        createGameFrame();
    }

    private void createGameFrame()
    {
        JFrame gameFrame = createFrame("Tic-Tac-Toe Game", 600, 800, false, true);

        gameFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        gameFrame.setIconImage(gameFrameImageIcon.getImage());
        
        // Adjust the layout as needed
        boardPanel = createPanelGridLayout(3, 3, 2, 2, Color.LIGHT_GRAY, 600, 600); 
        
        gameFrame.add(boardPanel);

        // Add all of the board buttons
        for(int index = 0; index < BOARD_SIZE * BOARD_SIZE; index++) 
        {
            array[index] = createButton(index % 2 == 0 ? new Color(16, 44, 84) : new Color(48, 101, 172), 200, 200);
            array[index].addActionListener(e -> {
                JButton clickedButton = (JButton) e.getSource();
            
                if(clickedButton.getIcon() == null) 
                {
                    clickedButton.setIcon(xImage);  // Player's move

                    UserMove(board, AI_USER);
                    clickedButton.setEnabled(false);
        
                    if(currentTurn < BOARD_SIZE * BOARD_SIZE) // Preventing the AI from making a move on a full board
                    {
                        int positionAIMove = AIMove(board);
        
                        array[positionAIMove - 1].setIcon(circleImage);
                        array[positionAIMove - 1].setEnabled(false);
                    }
                }
            });

            boardPanel.add(array[index]);
        }

        // Set the main bar for score, messages and reset button
        JPanel scorePanel = createPanelGridLayout(2, 2, Color.WHITE, 300, 120);
        
        // Set Player 1 score counter
        scoreMeassageLabelPlayer1 = createLabel("Score Player 1: " + currentScorePlayer1, 20);
        
        // Set Player 2 score counter
        scoreMeassageLabelPlayer2 = createLabel("Score Player 2: " + currentScorePlayer2, 20);
        
        // Set game message panel
        gameMessageLabel = createLabel(startingGameMessage, 25);

        // Set the reset button as default to be unabled
        resetButton = createButton("START NEW GAME", Color.RED, 300, 60, 25, false);

        // Add scorePanel and all the components in it
        gameFrame.add(scorePanel, BorderLayout.SOUTH);

        scorePanel.add(scoreMeassageLabelPlayer1, 0);
        scorePanel.add(gameMessageLabel, BorderLayout.CENTER, 1);
        scorePanel.add(scoreMeassageLabelPlayer2, 2);
        scorePanel.add(resetButton, 3);

        // Create a JPanel for the title
        //? Understand a universal way to center a text in a JPanel
        JPanel titlePanel = createPanelFlowLayout(FlowLayout.CENTER, 0, 25, Color.WHITE, 600, 80);
        
        // Create a JLabel with the desired text
        JLabel titleLabel = createLabel("Tic-Tac-Toe Game", 25);
        
        titlePanel.add(titleLabel);
        
        // Add the title panel to the frame
        gameFrame.add(titlePanel, BorderLayout.NORTH);
        
        // Center the frame on the screen
        gameFrame.setLocationRelativeTo(null);
        
        gameFrame.setVisible(true);

        resetButton.addActionListener(e -> resetGame());
    }

    protected JFrame createFrame(String title, int width, int height, boolean setResizable, boolean setVisible)
    {
        JFrame frame = new JFrame(title);
        frame.setSize(width,height);
        frame.setResizable(setResizable);
        frame.setVisible(setVisible);

        return frame;
    }

    // TODO: Implement this function 
    private void displayGameModeMenu() 
    {
        JFrame menuFrame = createFrame("Game Menu", 300, 500, false, true);

        // Add a WindowListener to handle the window closing event
        menuFrame.addWindowListener(new WindowAdapter() 
        {
            @Override
            public void windowClosing(WindowEvent e) 
            {
                // Close only the menuFrame when the menuFrame is closed
                menuFrame.dispose();
            }
        });

        menuFrame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);

        JPanel optionsPanel = createPanelGridLayout(3, 1, 2, 5, Color.WHITE, 300, 300);

        JButton menuButtons[] = new JButton[3];

        for(int i = 0; i < menuButtons.length; i++) 
        {
            menuButtons[i] = createButton(GAME_MODES[i], Color.WHITE, 300, 100, 25, false);

            menuButtons[i].addActionListener(e-> {
                    JButton clickedButton = (JButton) e.getSource();
                    
                    if(clickedButton.getText().equals(GAME_MODES[USER_VS_AI_LEVEL_1]))
                    {
                        System.out.println("USER_VS_AI_LEVEL_1 selected");
                    } 
                    else if(clickedButton.getText().equals(GAME_MODES[USER_VS_AI_LEVEL_2]))
                    {
                        System.out.println("USER_VS_AI_LEVEL_2 selected");
                    } 
                    else if(clickedButton.getText().equals(GAME_MODES[USER_VS_USER]))
                    {
                        System.out.println("USER_VS_USER_1 selected");
                    }
            });
        
            optionsPanel.add(menuButtons[i]);
        }

        menuFrame.add(optionsPanel);

        menuFrame.setLocationRelativeTo(null);
        menuFrame.setAlwaysOnTop(true);
    }
    
    private static int MiniMax(String[] board, String player)
    {
        if(CheckWin(board, AI))
        {
            return 1;
        }
        else if(CheckWin(board, AI_USER))
        {
            return -1;
        }
        else if(CheckTie(board))
        {
            return 0;
        }

        if(player == AI)
        {
            int globalVal = Integer.MIN_VALUE;

            for(int i = 1; i <= BOARD_SIZE * BOARD_SIZE; i++)
            {
                int val = Integer.MIN_VALUE;

                if(board[i - 1] == EMPTY_SPACE)
                {
                    board[i - 1] = AI;
                    val = MiniMax(board, AI_USER);
                    board[i - 1] = EMPTY_SPACE;
                }
                
                globalVal = Math.max(val, globalVal);
            }

            return globalVal;
        }
        else
        {
            int globalVal = Integer.MAX_VALUE;

            for(int i = 1; i <= BOARD_SIZE * BOARD_SIZE; i++)
            {
                int val = Integer.MAX_VALUE;

                if(board[i - 1] == EMPTY_SPACE)
                {
                    board[i - 1] = AI_USER;
                    val = MiniMax(board, AI);
                    board[i - 1] = EMPTY_SPACE;
                }
                
                globalVal = Math.min(val, globalVal);
            }

            return globalVal;
        }
    }

    private static int AIMove(String[] board) 
    {
        int bestVal = Integer.MIN_VALUE;
        int bestMove = -1;
    
        for(int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) 
        {
            int moveVal = Integer.MIN_VALUE;
    
            if(board[i] == EMPTY_SPACE) 
            {
                board[i] = AI;
                moveVal = MiniMax(board, AI_USER);
                board[i] = EMPTY_SPACE;
            }
    
            if(moveVal > bestVal) 
            {
                bestVal = moveVal;
                bestMove = i + 1;  // Adjust index to match user input (1 to 9)
            }
        }

        try 
        {
            Thread.sleep(DELAY_ONE_SECOND); // Simulate some delay for visualization
        } 
        catch (InterruptedException ex) 
        {
            ex.printStackTrace();
        }
    
        InsertMove(board, bestMove, AI);
    
        currentTurn++;
    
        return bestMove;
    }
    
    public static void UserMove(String[] board, String player)
    {   
        int position = getIndexClickedButton();

        InsertMove(board, position, player);

        currentTurn++;
    }

    private static boolean CheckWin(String[] board, String player) {
        // Check rows and columns
        for(int i = 0; i < BOARD_SIZE; i++) 
        {
            // Check rows
            if (board[i * BOARD_SIZE].equals(player) && 
                board[i * BOARD_SIZE + 1].equals(player) && 
                board[i * BOARD_SIZE + 2].equals(player)) 
            {
                return true;
            }
            // Check columns
            if (board[i].equals(player) && 
                board[i + BOARD_SIZE].equals(player) &&
                board[i + 2 * BOARD_SIZE].equals(player)) 
            {
                return true;
            }
        }
    
        // Check diagonals
        if (board[0].equals(player) && board[4].equals(player) && board[8].equals(player) || 
            board[2].equals(player) && board[4].equals(player) && board[6].equals(player)) 
        {
            return true;
        }
    
        return false;
    }
    

    private static boolean CheckWin(String[] board) 
    {
        for(int i = 0; i < BOARD_SIZE; i++) 
        {
            // Check rows
            if (board[i * BOARD_SIZE].equals(board[i * BOARD_SIZE + 1]) && 
                board[i * BOARD_SIZE].equals(board[i * BOARD_SIZE + 2])) 
            {
                return true;
            }

            // Check columns
            if (board[i].equals(board[i + BOARD_SIZE]) &&
                board[i].equals(board[i + (BOARD_SIZE * 2)])) 
            {
                return true;
            }
        }
    
        // Check diagonals
        if (board[0].equals(board[4]) && board[0].equals(board[8]) ||
            board[2].equals(board[4]) && board[2].equals(board[6])) 
        {
            return true;
        }
    
        return false;
    }
    
    
    private static boolean CheckTie(String[] board) 
    {
        for(int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) 
        {
            if(board[i] == EMPTY_SPACE) 
            {
                return false;
            }
        }

        return true;
    }

    private static void InsertMove(String[] board, int position, String player)
    {
        if((position >= 1) && ((position <= BOARD_SIZE * BOARD_SIZE)))
        {
            if(PositionEmpty(board, position))
            {
                board[position - 1] = player;

                if(CheckWin(board, player))
                {
                    updateGUIIfGameOver();
                }
                else if(CheckTie(board))
                {
                    updateGUIIfGameOver();
                }
            }
        }
    }

    private static boolean PositionEmpty(String[] board, int position) 
    {
        if(board[position - 1] == EMPTY_SPACE) 
        {
            return true;
        }

        return false;
    }

    private static void debugPrintBoard(String[] board)
    {
        System.out.println();

        for(int i=1; i<=BOARD_SIZE * BOARD_SIZE; i++)
        {
            System.out.print(board[i-1]);

            if(((i%3) == 0) && (i != BOARD_SIZE * BOARD_SIZE))
            {
                System.out.println("\n--+---+---");
            }
            else if(i != BOARD_SIZE * BOARD_SIZE){
                System.out.print(" | ");
            }
        }
        System.out.println();
    }

    public static int getIndexClickedButton() 
    {
        for(int index = 0; index < BOARD_SIZE * BOARD_SIZE; index++) 
        {
            if((array[index].getIcon() == xImage) && array[index].isEnabled()) 
            {
                return (index + 1);
            }
        }

        return -1; // No valid index found
    }


    public static boolean updateGUIIfGameOver()
    {
        if(currentTurn >= 5 && CheckWin(board))
        {
            setEndOfGameButtons();

            if(currentTurn % 2 == 1) 
            {
                gameMessageLabel.setText("Player 1 WON!");
                
                currentScorePlayer1++;
                scoreMeassageLabelPlayer1.setText("Score Player 1: " + currentScorePlayer1);
            } 
            else 
            {
                gameMessageLabel.setText("Player 2 (AI) WON!");
                
                currentScorePlayer2++;
                scoreMeassageLabelPlayer2.setText("Score Player 2: " + currentScorePlayer2);
            }
                        
            return true;
        } 
        else if(currentTurn == BOARD_SIZE * BOARD_SIZE) // It's a TIE
        {
            setEndOfGameButtons();

            gameMessageLabel.setText("It's a TIE!");

            return true;
        }      
    
        return false;
    }

    protected static void setEndOfGameButtons()
    {
        for(JButton button : array) 
        {
            button.setEnabled(false);
        }
        
        resetButton.setEnabled(true);
    }

    public void resetGame()
    {
        for(JButton button : array) 
        {
            button.setIcon(null);
            button.setEnabled(true);
        }
        
        gameMessageLabel.setText(startingGameMessage);

        setStartingBoard();
        
        currentTurn = 1;
    }
}
