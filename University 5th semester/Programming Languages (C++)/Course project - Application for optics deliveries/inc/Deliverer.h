#pragma once

#include <string>
#include <vector>

#include "Optics.h"

class Deliverer
{
private:
    // Information stored for every Deliverer object
    std::string bulstat;
    std::string name;
    std::string location;
    std::string phoneNumber;
    std::vector<Optics> opticsInStorage;

public:
    Deliverer() {}

    Deliverer(const std::string& bulstat, const std::string& name, const std::string& location, 
            const std::string& phoneNumber, const std::vector<Optics>& opticsInStorage)
        : bulstat(bulstat), name(name), location(location), 
            phoneNumber(phoneNumber), opticsInStorage(opticsInStorage) {}

    ~Deliverer() {}

    friend std::ostream& operator<<(std::ostream& os, const Deliverer& optics);

    void displayOptics() const;

    void addOptics(const Optics& optics) { opticsInStorage.push_back(optics); }

    std::string getBulstat() const { return bulstat; }
    std::string getName() const { return name; }
    std::string getLocation() const { return location; }
    std::string getPhoneNumber() const { return phoneNumber; }
    std::vector<Optics> getOpticsInStorage() const { return opticsInStorage; }

    void setBulstad(const std::string& bulstat) { this->bulstat = bulstat; }
    void setName(const std::string& name) { this->name = name; }
    void setLocation(const std::string& location) { this->location = location; }
    void setPhoneNumber(const std::string& phoneNumber) { this->phoneNumber = phoneNumber; }
    void setOpticsInStorage(const std::vector<Optics>& opticsInStorage) { this->opticsInStorage = opticsInStorage; }
};
