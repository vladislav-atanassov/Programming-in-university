import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintStream;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Pattern;

public class Regex extends RegexTester
{
    final protected String regexFile = "regexFile.txt";

    final public int indexOfPattern = 0;
    final public int indexOfDescription = 1;
    final public int indexOfRating = 2;
    final public int indexOfId = 3;

    private Pattern pattern; 
    private String description;
    private int rating;
    private int id;
    
    private static int nextId = 0;

    Regex() {}
    
    Regex(Pattern pattern, String description)
    {
        setPattern(pattern);
        setDescription(description);
        rating = 0;
        id = ++nextId;
    }

    Regex(Pattern pattern, String description, int rating, int id) 
    {
        setPattern(pattern);
        setDescription(description);
        setRating(rating);
        setId(id);
    }

    public List<String> readRegex() 
    {
        List<String> regexs = new ArrayList<>();

        try(BufferedReader br = new BufferedReader(new FileReader(regexFile))) 
        {
            String str;

            while((str = br.readLine()) != null) 
            {
                regexs.add(str);
            }
        } 
        catch(IOException e) 
        {
            e.printStackTrace();
        }

        return regexs;
    }

    public void writeRegex(Regex regex, PrintStream out) throws IOException
    {
        List<String> fileContent = readRegex();

        int lastId = -1;
        String[] arrayElemStrings = null;

        for(String listElement : fileContent) 
        {
            arrayElemStrings = listElement.split(", ");

            if(arrayElemStrings[indexOfPattern].equals(regex.getPattern().toString()))
            {
                out.printf("The pattern: (%s) already exists!\n", regex.getPattern().toString());
                return;
            }  
        }
        
        lastId = Integer.parseInt(arrayElemStrings[indexOfId]);
        
        try(BufferedWriter f_writer = new BufferedWriter(new FileWriter(regexFile, true));) 
        {
            f_writer.write(regex.getPattern() + ", ");
            f_writer.write(regex.getDescription() + ", ");
            f_writer.write(regex.getRating() + ", ");
            f_writer.write((lastId + 1) + "");
            
            f_writer.newLine();
            f_writer.close();
        }
        catch(IOException e)
        {
            e.printStackTrace();
        }
    }

    public List<Regex> filterByKeyWord(List<Regex> regexs, String keyWord)
    {
        List<String> fileContent = readRegex();

        for(String listElement : fileContent) 
        {
            String[] arrayElemStrings = listElement.split(", ");

            if(arrayElemStrings[indexOfDescription].toLowerCase()
                .contains(keyWord.toLowerCase()))
            {
                regexs.add(new Regex(
                        Pattern.compile(arrayElemStrings[indexOfPattern]), 
                        arrayElemStrings[indexOfDescription], 
                        Integer.parseInt(arrayElemStrings[indexOfRating]), 
                        Integer.parseInt(arrayElemStrings[indexOfId])));
            }            
        }  

        return regexs;
    }

    public String getDescription() {
        return description;
    }

    public int getId() {
        return id;
    }

    public static int getNextId() {
        return nextId;
    }

    public Pattern getPattern() {
        return pattern;
    }

    public int getRating() {
        return rating;
    }

    public void setDescription(String description) {
        this.description = description;
    }

    public void setId(int id) {
        this.id = id;
    }

    public void setPattern(Pattern pattern) {
        this.pattern = pattern;
    }

    public void setRating(int rating) {
        this.rating = rating;
    }
}
