import java.io.IOException;
import java.io.PrintStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Queue;
import java.util.Scanner;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Server 
{
    ServerSocket server = null;
    final String fileName = "players.data.txt";
    Queue<Connection> playersQueue = null;

    // enum for the loadPlayers function
    enum IndexesInFile
    {
        indexName,
        indexRating,
        indexPassword,
        indexIsPaying
    }

    Player[] loadPlayers()
    {
        // Read from fileName = "players.data.txt"
        // Traverse the whole file and add to the array of Players

        // [Misho, 1224, true], [], ...

        return null;
    }

    Player login(String name, String password)
    {
        // Check if there is a player with that name and password
        // Call loadPlayers() and validate for name and password

        // else
        return null;
    }

    Connection getPlayer2(Connection player1)
    {
        for(Connection playerConnection : playersQueue)
        { 
        //  if((abs(player1.getRating() - playerConnection.player.getRating()) <= 200) &&
        //      playerConnection.player.getIsPlaying() == false)
        //  {
        //      return playerConnection;
        //  }
        }        

        // if no valid player is found send message and return null
        System.out.println("There are no players at the moment. Pleaase wait!");
        return null;
    }

    boolean isValidMove(String move)
    {
        Pattern pattern = Pattern.compile("[A-Z]{0,1}+[x]{0,1}+[a-h]{1}+[1-8]{1}+[+|#]{0,1}");
        Matcher matcher = pattern.matcher(move);

        if(matcher.matches())
        {
            return true;
        }

        return false;
    }

    public Connection choosePiecesAfterLogIn(PrintStream out, Scanner in)
    {
        // Get input for name and password
        // to be used in the login function

        // Player player = login(name, password)

        // if(player != null)
        // {
        //    Connection newConnection = new Conncetion(player, out, in);
        // }
        // else
        // {
        //    return;
        // }

        // Get input for choosing black or white pieces
        // if(black chosen)
        // {
        //     playersQueue.add(player);
        // }

        // return newConnection;
    }

    public void startGame(PrintStream out, Scanner in)
    {
        Connection connection = choosePiecesAfterLogIn(out, in);
        Player player = connection.player;

        while(true)
        {
            if(connection.reciveMessage().contains("#"))
            {
            //  player.setRating(player.getRating() - 10);
            //  return;
            }
            
            connection.sendMessage(in.nextLine());
        }
    }

    public void start() 
    {
        try 
        {
            System.out.println("Server listening.");
            server = new ServerSocket(8080);
            
            while(true) 
            {
                Socket client = server.accept();

                Thread clientThread = new Thread(() -> 
                {
                    System.out.println("Accepted client.");
                    Scanner in = null;
                    PrintStream out = null;

                    try 
                    {
                        in = new Scanner(client.getInputStream());
                        out = new PrintStream(client.getOutputStream());

                        startGame(out, in);
                    } 
                    catch(IOException e) 
                    {
                        e.printStackTrace();
                    } 
                    finally 
                    {
                        if(in != null)
                            in.close();
                        if(out != null)
                            out.close();
                    }
                });

                clientThread.start();
            }
        } 
        catch(IOException e) 
        {
            e.printStackTrace();
        }
    }
}
