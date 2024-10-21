#include "your_path_to_directoryDeliverer.h"

std::ostream& operator<<(std::ostream& os, const Deliverer& deliverer)
{
    os << "Deliverer Details:" << std::endl;
    os << "Bulstad: " << deliverer.getBulstat() << std::endl;
    os << "Name: " << deliverer.getName() << std::endl;
    os << "Location: " << deliverer.getLocation() << std::endl;
    os << "Phone Number: " << deliverer.getPhoneNumber() << std::endl;

    os << "\nOptics In Storage:" << std::endl;

    for(const Optics& optics : deliverer.getOpticsInStorage()) 
    {
        os << optics << std::endl; 
    }

    return os;
}

void Deliverer::displayOptics() const
{
    if(opticsInStorage.empty())
    {
        std::cout << "No optics in storage." << std::endl;
        return;
    }

    std::cout << "Optics in Storage:" << std::endl;

    for(size_t i = 0; i < opticsInStorage.size(); ++i) 
    {
        std::cout << i + 1 << ". " << opticsInStorage[i] << std::endl;
    }
}
