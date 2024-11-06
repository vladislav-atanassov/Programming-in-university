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
    const std::string ordersLogFileName = "../Log_files/ordersLogFile.txt";
    const std::string deliverersLogFileName = "../Log_files/deliverersLogFile.txt";

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
