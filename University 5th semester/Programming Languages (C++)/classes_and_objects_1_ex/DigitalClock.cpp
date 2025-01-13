#include <iostream>
#include <iomanip>

constexpr int fullHours = 0;
constexpr int halfHours = 1;
constexpr int errCodeMinutesorHours = 12;
constexpr int errCodeHours = 22;

class DigitalClock
{
private:
    void errors() const
    {
        if((minutes > 59 || minutes < 0) || 
            (seconds > 59 || seconds < 0))
        {
            std::cerr << "Imposible seconds or minutes!" << std::endl;
            exit(errCodeMinutesorHours);
        }

        if((mode == fullHours) && ((hour > 23) || (hour < 0)) ||
            (mode == halfHours) && ((hour > 11) || (hour < 0)))   
        {
            std::cerr << "Imposible hour!" << std::endl;
            exit(errCodeHours);
        }
    }

public:
    int hour;
    int minutes;
    int seconds;
    bool mode;

    DigitalClock() : hour(0), minutes(0), seconds(0), mode(fullHours) {}
    ~DigitalClock() {}

    void inputHour();
    void displayHour();
}; 

void DigitalClock::inputHour()
{
    int hour;
    int minutes;
    int seconds;

    std::cout << "Enter hours: ";
    std::cin >> hour;
    std::cout << "Enter minutes: ";
    std::cin >> minutes;
    std::cout << "Enter seconds: ";
    std::cin >> seconds;
    std::cout << "Enter mode: ";
    std::cin >> mode;

    this->errors();

    this->hour = hour;
    this->minutes = minutes;
    this->seconds = seconds;
    this->mode = mode;
}

void DigitalClock::displayHour()
{
    if(this->mode == fullHours)
    {
        std::cout << hour << ":" << std::setfill('0') << std::setw(2) << minutes << ":" 
                << std::setfill('0') << std::setw(2) << seconds << std::endl;
        return;
    }

    if(this->mode == halfHours)
    {
        if(hour == 12 && minutes == 0 && seconds == 0)
        {
            std::cout << hour << ":" << std::setfill('0') << std::setw(2) << minutes << ":" 
                << std::setfill('0') << std::setw(2) << seconds << " AM" << std::endl;
        }

        if(hour > 12)
        {
            hour = hour - 12;
            std::cout << hour << ":" << std::setfill('0') << std::setw(2) << minutes << ":" 
                << std::setfill('0') << std::setw(2) << seconds << " PM" << std::endl;
        }
        else
        {
            std::cout << hour << ":" << std::setfill('0') << std::setw(2) << minutes << ":" 
             << std::setfill('0') << std::setw(2) << seconds << " PM" << std::endl; 
        }
    }
}

int main()
{
    DigitalClock digitalClockObj;
    
    digitalClockObj.inputHour();
    digitalClockObj.displayHour();
}
