#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "Deliverer.h"
#include "Optics.h"

class OrderSystem 
{
private:
    const std::string ordersLogFileName = "C:\\Users\\Vladislav Atanassov\\Documents\\Programming in university\\University 5th semester (C++)\\Course project - Application for optics deliveries\\Log_Files\\ordersLogFile.txt";
    const std::string deliverersLogFileName = "C:\\Users\\Vladislav Atanassov\\Documents\\Programming in university\\University 5th semester (C++)\\Course project - Application for optics deliveries\\Log_Files\\deliverersLogFile.txt";

    std::vector<Deliverer> deliverers;

public:
    OrderSystem() {}

    OrderSystem(const std::vector<Deliverer>& deliverers) : deliverers(deliverers) {}

    ~OrderSystem() {}

    void addDeliverer(const Deliverer& deliverer) 
    { 
        deliverers.emplace_back(deliverer); 
        saveDelivererToFile(deliverer); 
    }

    void displayDelivereres() const;

    // Appends a sigle Deliverer in the file
    void saveDelivererToFile(const Deliverer& deliverver) const; 
    // Deletes the whole file and writes the current data
    void saveDeliverersToFile() const;
    // Appends a single order in the file
    void saveOrderToFile(const Deliverer& deliverer, const std::vector<Optics>& orderedOptics, float totalPrice) const;

    Deliverer selectDeliverer() const;
    
    void placeOrder();

    std::vector<Deliverer> getDeliverers() const { return deliverers; }
    const std::string getOrdersLogFile() const { return ordersLogFileName; } 

    void setDeliverers(const std::vector<Deliverer>& deliverers) { this->deliverers = deliverers; }
};

// void OrderSystem::displayDelivereres() const 
// {
//     if(deliverers.empty()) 
//     {
//         std::cout << "No deliverers available." << std::endl;
//         return;
//     }

//     for(size_t i = 0; i < deliverers.size(); i++)
//     {
//         std::cout << i + 1 << ". " << deliverers[i].getName() << std::endl;
//     }
// }

// void OrderSystem::saveDelivererToFile(const Deliverer& deliverer) const
// {
//     std::ofstream outFile(deliverersLogFileName, std::ios::app);
    
//     if (!outFile) 
//     {
//         std::cerr << "Error opening file: " << deliverersLogFileName << std::endl;
//         return;
//     }

//     outFile << deliverer << std::endl;

//     outFile.close();
// }


// void OrderSystem::saveDeliverersToFile() const
// {
//     std::ofstream outFile(deliverersLogFileName, std::ios::trunc);

//     if (!outFile) 
//     {
//         std::cerr << "Error opening file: " << deliverersLogFileName << std::endl;
//         return;
//     }

//     std::vector<Deliverer> deliverers = this->getDeliverers();

//     for(const auto& deliverer : deliverers) 
//     {
//         outFile << deliverer << std::endl;
//     }

//     outFile.close();
//     std::cout << "Deliverers saved to " << deliverersLogFileName << std::endl;
// }

// void OrderSystem::saveOrderToFile(const Deliverer& deliverer, const std::vector<Optics>& orderedOptics, float totalPrice) const
// {
//     std::ofstream ordersFile(ordersLogFileName, std::ios::app);
    
//     if(!ordersFile) 
//     {
//         std::cerr << "Error opening file: " << ordersLogFileName << std::endl;
//         return;
//     }

//     ordersFile << "Order Details:\n";
//     ordersFile << "Deliverer: " << deliverer.getName() << "\n";
//     ordersFile << "Optics Ordered:\n";

//     for(const auto& optic : orderedOptics) 
//     {
//         ordersFile << " - " << optic.getType() << " (Price: " << optic.getPrice() << " BGN)\n";
//     }
    
//     ordersFile << "Total Price: " << totalPrice << " BGN\n";
//     ordersFile << "------------------------------------\n";

//     ordersFile.close();
// }

// Deliverer OrderSystem::selectDeliverer() const
// {
//     this->displayDelivereres();

//     std::cout << "Select a deliverer by number: ";
    
//     size_t delivererIndex;
//     std::vector<Deliverer> deliverers = this->getDeliverers();

//     while(true)
//     {
//         std::cin >> delivererIndex;

//         if(delivererIndex < 1 || delivererIndex > deliverers.size()) 
//         {
//             std::cout << "Invalid deliverer choice. Try Again: " << std::endl;
//             continue;
//         }

//         break;
//     }

//     Deliverer selectedDeliverer = deliverers[delivererIndex - 1];
//     std::cout << "You have selected: " << selectedDeliverer.getName() << std::endl;

//     return selectedDeliverer;
// }

// void OrderSystem::placeOrder() 
// {
//     if(deliverers.empty()) 
//     {
//         std::cout << "No deliverers available to place an order." << std::endl;
//         return;
//     }

//     Deliverer selectedDeliverer = this->selectDeliverer();

//     selectedDeliverer.displayOptics();
//     const std::vector<Optics>& optics = selectedDeliverer.getOpticsInStorage();

//     std::cout << "Enter the number of the optic to order (or 0 to finish): ";
//     size_t choice;
//     float totalPrice = 0.0f;
//     std::vector<Optics> orderedOptics = {};

//     while(true) 
//     {
//         std::cin >> choice;

//         if(choice == 0) 
//         {
//             break;
//         }
        
//         if(choice < 1 || choice > optics.size()) 
//         {
//             std::cout << "Invalid choice. Try again: ";
//             continue;
//         }

//         orderedOptics.emplace_back(optics[choice - 1]);
//         totalPrice += optics[choice - 1].getPrice();

//         std::cout << "Optic added to order. Current total: " << totalPrice << " BGN." << std::endl;
//         std::cout << "Select another optic or 0 to finish: ";
//     }

//     saveOrderToFile(selectedDeliverer, orderedOptics, totalPrice);
    
//     std::cout << "Order complete. Total price: " << totalPrice << " BGN." << std::endl;
// }

