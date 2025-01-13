#pragma once

#include <iostream>

#include "Deliverer.h"
#include "Optics.h"
#include "InputHandler.h"
#include "OrderSystem.h"

class ApplicationStarter
{
private:
    enum MenuInputCases
    {
        ADD_DELIVERER = 1,
        ADD_OPTICS_TO_DELIVERER,
        PLACE_ORDER,
        EXIT
    };

public:
    ApplicationStarter() {}
    ~ApplicationStarter() {}

    void testInput(OrderSystem& orderSystem);
    // Dispays the functional options to the client
    void mainMenu(InputHandler& ioHandler, OrderSystem& orderSystem);
    // Initializes InputHandler and OrderSystem and calls mainMenu()
    void start();
};
