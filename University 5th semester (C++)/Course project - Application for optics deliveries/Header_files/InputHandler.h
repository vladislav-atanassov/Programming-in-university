#pragma once

#include <iostream>

#include "Deliverer.h"
#include "Optics.h"
#include "OrderSystem.h"

class InputHandler
{
public:
    InputHandler() {}
    
    ~InputHandler() {}

    Optics inputNewOptics();

    void inputOpticsToDeliverer(OrderSystem& orderSystem);

    void inputNewDeliverer(OrderSystem& orderSystem);
};
