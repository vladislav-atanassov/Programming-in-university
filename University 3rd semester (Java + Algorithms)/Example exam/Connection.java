import java.io.PrintStream;
import java.util.Scanner;

public class Connection 
{
    Player player;
    PrintStream out;
    Scanner in;   

    Connection(Player player, PrintStream out, Scanner in)
    {
        this.player = player;
        this.out = out;
        this.in = in;
    }
    
    public void sendMessage(String message)
    {
        out.println(message);
    }

    public String reciveMessage()
    {
        return in.nextLine();   
    }
}
