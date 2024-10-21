#include <iostream>
#include <string>
#include <vector>

class Worker
{
private:
    int socNumber;
    std::string name;
    int workExp;
    std::string currPosition;
    std::vector<double> salaries;  

public:

    Worker() : socNumber(0), workExp(0) {}
    Worker(int pos)
    {
        std::cout << "Enter currnet position: ";
        std::cin >> pos;

        this->currPosition = pos;
    }

    ~Worker() {};

    double agrSalary() const
    {   
        double sum = 0;

        for(double s : salaries)
        {
            sum += s;
        }

        return static_cast<double>(sum / salaries.size());
    }

    double minSalary() const
    {
        double tempMinSal = salaries[0];

        for(double s : salaries)
        {
            if(s < tempMinSal)
            {
                tempMinSal = s;
            }
        }
    }
};

