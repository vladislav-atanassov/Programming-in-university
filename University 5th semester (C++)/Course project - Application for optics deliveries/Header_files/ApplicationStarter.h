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

    void mainMenu(InputHandler& ioHandler, OrderSystem& orderSystem);

    void start();
};
