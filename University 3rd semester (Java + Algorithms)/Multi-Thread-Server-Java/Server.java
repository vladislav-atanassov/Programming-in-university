import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;
import java.util.Scanner;
import java.util.regex.Pattern;
import java.util.regex.PatternSyntaxException;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintStream;
import java.net.ServerSocket;
import java.net.Socket;

public class Server extends Regex 
{
    private ServerSocket server;
    private final Object createRegexLock;

    public static enum ClientOptions
    {
        CREATE_REGEX,
        SEARCH_REGEX
    }

    Server() 
    { 
        super(); 
        createRegexLock = new Object();
    }

    public void clientMenu(Scanner sc, PrintStream out) 
    {
        out.println("=== Client Menu ===");
        out.println("1. Create Regex");
        out.println("2. Search Regex");
        out.println("===================");
    
        int choice = 0;
        boolean isNotFinished = true;

        while(isNotFinished) 
        {
            out.println("Enter your choice (1-2): ");
    
            choice = sc.nextInt();
            sc.nextLine();
    
            if(choice != 1 && choice != 2) 
            {
                out.println("Invalid choice. Please try again.");
                continue;
            }
    
            switch(ClientOptions.values()[choice - 1]) 
            {
                case CREATE_REGEX:
                    clientCreateRegex(sc, out);
                    isNotFinished = false;
                    break;
                case SEARCH_REGEX:
                    clientSearchRegex(sc, out);
                    isNotFinished = false;
                    break;
            }
        }
    }
    
    public void clientCreateRegex(Scanner sc, PrintStream out) 
    {
        Pattern regexPattern = getInputRegexPattern(sc, out);
        String description = getInputDescription(sc, out);
        String[] testInputs = getInputTestInputs(sc, out);

        Regex regex = new Regex(regexPattern, description);
        
        out.println(test(regex, testInputs).toString());
            
        try 
        {
            out.println("Would you like to add this Regex to the file? Y/N: ");
            
            if((sc.nextLine()).toLowerCase().equals("y")) 
            {
                synchronized(createRegexLock)
                {
                    regex.writeRegex(regex, out);
                }
            }
        } 
        catch(IOException e) 
        {
            e.printStackTrace();
        }
        finally
        {
            sc.close();
        }
    }
    
    private Pattern getInputRegexPattern(Scanner sc, PrintStream out) 
    {
        while(true) 
        {
            out.println("Enter the regex pattern: ");
            String patternStr = sc.nextLine();
    
            // Check if the Pattern has syntax errors
            try  
            {
                Pattern pattern = Pattern.compile(patternStr); 
                return pattern; 
            } 
            catch(PatternSyntaxException e) 
            {
                out.print("The pattern has a syntax error: ");
                out.println(e.getMessage());
            }
        }
    }

    
    private String getInputDescription(Scanner sc, PrintStream out) 
    {
        out.println("Enter the description: ");
        String description = sc.nextLine();

        return description;
    }

    private String[] getInputTestInputs(Scanner sc, PrintStream out) 
    {
        out.println("Enter the number of test inputs: ");

        int numTestInputs = sc.nextInt();
        sc.nextLine();  // Consume the newline character
    
        String[] testInputs = new String[numTestInputs];
    
        for(int currTest = 0; currTest < numTestInputs; currTest++) 
        {
            out.printf("Enter test input No %d.: \n", currTest + 1);

            testInputs[currTest] = sc.nextLine();
        }
    
        return testInputs;
    }
            
    public void clientSearchRegex(Scanner sc, PrintStream out)
    {
        List<Regex> regexs = new ArrayList<>();

        while(true)
        {
            out.println("Enter key word: ");
            String keyWord = sc.nextLine();
            
            filterByKeyWord(regexs, keyWord); // Function of the Regex class
            
            if(regexs.isEmpty())
            {
                out.println("There is not match for your key word!");
                continue;
            }
            
            break;
        }

        // Sort the list in descending order
        Collections.sort(regexs, 
                Comparator.comparingInt(Regex::getRating).reversed());

        for(Regex regex : regexs) 
        {
            out.printf("%s with ID: %d\n", 
                    regex.getPattern().toString(), regex.getId());
        }

        boolean isNotFinished = true;
        while(isNotFinished)
        {
            out.println("Enter the ID of the pattern you want to use: ");
            
            int wantedPatternID = Integer.parseInt(sc.nextLine());
            
            // Check if the ID is valid and if so continue the program
            for(Regex regex : regexs) 
            {
                if(regex.getId() == wantedPatternID) 
                {
                    out.println(test(regex, getInputTestInputs(sc, out)));
                    out.println("Do you want to change the rating of this pattern (Y/N): ");
                    if((sc.nextLine()).toLowerCase().equals("y")) 
                    {
                        out.println("Do you want to increment or decrement the rating (I/D): ");
                        String choice = sc.nextLine().toLowerCase();
                        if(choice.equals("i")) 
                        {
                            updateRatingInFile(regex, regex.getId(), (regex.getRating() + 1));
                        } 
                        else if(choice.equals("d")) 
                        {
                            updateRatingInFile(regex, regex.getId(), (regex.getRating() - 1));
                        }
                    }
                    
                    break;
                }
            }

            if(isNotFinished)
                out.println("The ID you entered is incorrect!");
        }
    }

    // Method to update the rating in the file
    private void updateRatingInFile(Regex regex, int regexID, int newRating) 
    {
        List<String> fileContent = readRegex();
        StringBuilder updatedContent = new StringBuilder();

        for(String line : fileContent) 
        {
            if(line.contains(regex.getDescription()) && 
                (Integer.parseInt("" + line.charAt(line.length() - 1)) == regexID)) 
            {
                // Find and update the rating in the line
                String updatedLine = line.replaceFirst(" " + regex.getRating() + ", ", " " + newRating + ", ");  
                updatedContent.append(updatedLine).append(System.lineSeparator());
                regex.setRating(newRating);
            } 
            else 
            {
                updatedContent.append(line).append(System.lineSeparator());
            }
        }

        try(BufferedWriter f_writer = new BufferedWriter(new FileWriter(regexFile))) 
        {
            synchronized(createRegexLock)
            {
                f_writer.write(updatedContent.toString());
            }
        } 
        catch(IOException e) 
        {
            e.printStackTrace();
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
                    Scanner sc = null;
                    PrintStream out = null;

                    try 
                    {
                        sc = new Scanner(client.getInputStream());
                        out = new PrintStream(client.getOutputStream());
                        clientMenu(sc, out);
                    } 
                    catch(IOException e) 
                    {
                        e.printStackTrace();
                    } 
                    finally 
                    {
                        if(sc != null)
                            sc.close();
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