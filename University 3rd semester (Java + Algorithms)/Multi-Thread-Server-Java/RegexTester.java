import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;

public class RegexTester 
{
    public static List<Boolean> test(Regex regex, String[] strings) 
    {
        List<Boolean> results = new ArrayList<>();

        for(String str : strings) 
        {
            Matcher matcher = regex.getPattern().matcher(str);
            results.add(matcher.matches());
        }

        return results;
    }

}
