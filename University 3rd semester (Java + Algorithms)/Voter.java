import java.util.List;

public class Voter implements SortingVoters
{
    public String name;
    public String city;
    public String street;
    public String address;

    Voter(String name,String city, String address, String street)
    {
        setName(name);
        setCity(city);
        setAddress(street);
        setAddress(street);
    }

    public void setName(String name)
    {
        this.name = name;
    }

    public void setCity(String city)
    {
        this.city = city;
    }

    public void setstreet(String street)
    {
        this.street = street;
    }

    public void setAddress(String address)
    {
        this.address = address;
    }

    public String getName()
    {
        return this.name;
    }

    public String getCity()
    {
        return this.city;
    }

    public String getstreet()
    {
        return this.street;
    }

    public String getAddress()
    {
        return this.street;
    }        

    @Override
    public void sorted(List<Voter> arrVoters) 
    {
        for(int i = 0; i < arrVoters.size(); i++)
        {
            if(arrVoters.get(i).address.equals("Sofia"))
            {
                arrVoters.remove(i);
            }
        }   
    }

    @Override
    public void filter(String str) 
    {
        
    }

    @Override
    public void thenComparing() 
    {
        
    }
}

