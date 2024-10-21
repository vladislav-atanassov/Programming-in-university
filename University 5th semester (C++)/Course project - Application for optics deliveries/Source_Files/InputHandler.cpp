#include "C:\Users\Vladislav Atanassov\Documents\Programming in university\University 5th semester (C++)\Course project - Application for optics deliveries\Header_files\InputHandler.h"

Optics InputHandler::inputNewOptics()
{
    std::string type, material;
    float thickness, diopter, price;
    std::cout << "Enter Optics Type: ";
    std::cin >> type;
    std::cout << "Enter Thickness (in mm): ";
    std::cin >> thickness;
    std::cout << "Enter Diopter: ";
    std::cin >> diopter;
    std::cout << "Enter Material: ";
    std::cin >> material;
    std::cout << "Enter Price (in BGN): ";
    std::cin >> price;

    return Optics(type, material, thickness, diopter, price);
}

void InputHandler::inputOpticsToDeliverer(OrderSystem& orderSystem)
{   
    std::vector<Deliverer> deliverers = orderSystem.getDeliverers();

    if(deliverers.empty()) 
    {
        std::cout << "No deliverers available to add optics." << std::endl;
        return;
    }

    Deliverer selectedDeliverer = orderSystem.selectDeliverer();

    Optics newOptics = this->inputNewOptics();

    for(size_t i = 0; i < deliverers.size(); i++) 
    {
        if(deliverers[i].getBulstat() == selectedDeliverer.getBulstat())
        {
            deliverers[i].addOptics(newOptics);
            break;
        }
    }

    orderSystem.setDeliverers(deliverers);

    orderSystem.saveDeliverersToFile();
}


void InputHandler::inputNewDeliverer(OrderSystem& orderSystem)
{
    std::string bulstat, name, location, phoneNumber;
    std::vector<Optics> optics = {};
    std::cout << "Enter Bulstat: ";
    std::cin >> bulstat;
    std::cout << "Enter Name: ";
    std::cin.ignore();
    std::getline(std::cin, name);
    std::cout << "Enter Location: ";
    std::getline(std::cin, location);
    std::cout << "Enter Phone Number: ";
    std::getline(std::cin, phoneNumber);
    Deliverer deliverer(bulstat, name, location, phoneNumber, optics);

    // Add optics to the Deliverer
    int numOptics;
    std::cout << "How many optics will the Deliverer provide? ";
    std::cin >> numOptics;

    for(int i = 0; i < numOptics; i++) 
    {
        Optics tempOptics = inputNewOptics();    
        deliverer.addOptics(tempOptics);
    }

    orderSystem.addDeliverer(deliverer);
}