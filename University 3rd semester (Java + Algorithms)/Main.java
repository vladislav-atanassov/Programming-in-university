import java.util.ArrayList;
import java.util.List;

public class Main 
{
    public static void main(String[] args) 
    {
        List<Voter> arrVoter = new ArrayList<>();

        arrVoter.add(new Voter("Petko", "Sofia", "Tuk", "Chavdaritsa"));
        arrVoter.add(new Voter("Petko", "Lumeno", "Tam", "Nukiga"));
        arrVoter.add(new Voter("Petko", "Lumeno", "Tam", "Nukiga"));
        arrVoter.add(new Voter("Vasko", "Sofia", "Tuk", "Gurko"));

    }
}
