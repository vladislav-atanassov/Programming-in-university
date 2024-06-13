import java.io.IOException;
import java.io.PrintStream;
import java.net.Socket;
import java.util.Scanner;
import java.util.regex.Pattern;
import java.util.regex.PatternSyntaxException;
import java.util.ArrayList;
import java.util.List;

public class Client extends Server
{
    public static enum ClientOptions 
    {
        CREATE_REGEX,
        SEARCH_REGEX
    }

    public static void main(String[] args) 
    {
        Socket server = null;
        Scanner console = null;
        Scanner sc = null;

        try 
        {
            server = new Socket("localhost", 8080);

            console = new Scanner(System.in);

            sc = new Scanner(server.getInputStream());
            PrintStream out = new PrintStream(server.getOutputStream());

            userMenu(console, sc, out);
        } 
        catch(IOException e) 
        {
            e.printStackTrace();
        } 
        finally 
        {
            if(server != null) 
            {
                try 
                {
                    server.close();
                } 
                catch(IOException e) 
                {
                    e.printStackTrace();
                }
            }
            if(console != null)
                console.close();
            if(sc != null)
                sc.close();
        }
    }

    private static void userMenu(Scanner console, Scanner sc, PrintStream out) {

        // Print Client Menu
        System.out.println(sc.nextLine());
        System.out.println(sc.nextLine());
        System.out.println(sc.nextLine());
        System.out.println(sc.nextLine());

        // Enter choice of action
        System.out.println(sc.nextLine());
        
        String option = console.nextLine();
        out.println(option);

        if (option.equals("Goodbye."))
            return;

        boolean isFinished = true;

        while(isFinished) 
        {
            // Check if if the option is valid
            if (Integer.valueOf(option) != 1 &&
                Integer.valueOf(option) != 2) 
            {
                System.out.println(sc.nextLine());
                continue;
            }

            switch (ClientOptions.values()[Integer.valueOf(option) - 1]) 
            {
                case CREATE_REGEX:
                    clientCreateRegex(console, sc, out);
                    isFinished = false;
                    break;
                case SEARCH_REGEX:
                    clientSearchRegex(console, sc, out);
                    isFinished = false;
                    break;
            }
        }
    }

    private static void clientCreateRegex(Scanner console, Scanner sc, PrintStream out) 
    {
        while(true) 
        {
            // Enter regex pattern
            System.out.println(sc.nextLine());
            String patternStr = console.nextLine();
            out.println(patternStr);
            
            // Check if the Pattern has syntax errors
            try 
            {
                Pattern.compile(patternStr);
                break;
            } 
            catch(PatternSyntaxException e) 
            {
                // Print the part of the stack trace 
                // that shows where is the error in the pattern
                System.out.println(sc.nextLine());
                System.out.println(sc.nextLine());
                System.out.println(sc.nextLine());
            }
        }

        // Enter description
        System.out.println(sc.nextLine());
        out.println(console.nextLine());

        // Enter number input tests
        System.out.println(sc.nextLine());
        
        int numberOfTests = Integer.parseInt(console.nextLine());
        out.println(numberOfTests);
        
        // Enter input tests
        for(int i = 0; i < numberOfTests; i++) 
        {
            System.out.println(sc.nextLine());
            out.println(console.nextLine());
        }
        
        // Print result of the testing
        System.out.println(sc.nextLine());
        
        // Enter Y/N
        System.out.println(sc.nextLine());
        out.println(console.nextLine());
    }
    
    private static void clientSearchRegex(Scanner console, Scanner sc, PrintStream out) 
    {
        while(true)
        {
            // Enter key word
            System.out.println(sc.nextLine());
            
            String keyWord = console.nextLine();
            out.println(keyWord);
            
            // Print the matching patterns
            Regex tempRegex = new Regex();
            List<Regex> filteredRegexs = tempRegex.filterByKeyWord(new ArrayList<>(), keyWord);

            int numberOfMaches = filteredRegexs.size();

            if(numberOfMaches == 0)
            {
                System.out.println(sc.nextLine());
                continue;
            }
            
            for(int i = 0; i < numberOfMaches; i++)
            {
                System.out.println(sc.nextLine());
            }
            
            boolean isNotFinished = true;

            while(isNotFinished)
            {
                // Enter the wanted ID
                System.out.println(sc.nextLine());
                int wantedPatternID = Integer.parseInt(console.nextLine());
                out.println(wantedPatternID);
                
                // Check if the ID is valid
                for(Regex regex : filteredRegexs)
                {
                    if(regex.getId() == wantedPatternID)
                    {
                        isNotFinished = false;
                        break;
                    }
                }

                if(isNotFinished)
                    System.out.println(sc.nextLine());
            }

            break;
        }

        // Enter input tests
        System.out.println(sc.nextLine());
        int numberOfTests = Integer.parseInt(console.nextLine());
        out.println(numberOfTests);

        for(int i = 0; i < numberOfTests; i++) 
        {
            System.out.println(sc.nextLine());
            out.println(console.nextLine());
        }

        // Print the result from the tests
        System.out.println(sc.nextLine());

        // Enter for option to change the rating
        System.out.println(sc.nextLine());
        String Y_N_Choice = console.nextLine();
        out.println(Y_N_Choice);

        if(Y_N_Choice.toLowerCase().equals("y"))
        {
            // Enter for increment of decrement
            System.out.println(sc.nextLine());
            String I_D_Choice = console.nextLine();
            out.println(I_D_Choice);
        }
    }
}
