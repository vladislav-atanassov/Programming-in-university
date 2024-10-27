#include "C:\Users\Vladislav Atanassov\Documents\Programming in university\University 5th semester (C++)\Course project - Application for optics deliveries\Header_files\ApplicationStarter.h"

void ApplicationStarter::testInput(OrderSystem& orderSystem)
{
    Optics optics1("Lens", "Glass", 5.0f, 2.5f, 100.0f);
    Optics optics2("Prism", "Plastic", 10.0f, 1.5f, 150.0f);
    Optics optics3("Mirror", "Metal", 8.0f, 0.0f, 200.0f);

    std::vector<Optics> opticsInStorage1 = { optics1, optics2, optics3 };

    Deliverer deliverer1("123456789", "Optics Deliveries Inc.", "Sofia, Bulgaria", 
                          "+359 88 123 4567", opticsInStorage1);

    Optics optics4("Lens", "Polycarbonate", 6.0f, 1.5f, 120.0f);
    Optics optics5("Filter", "Glass", 4.0f, 0.0f, 80.0f);
    Optics optics6("Coating", "Plastic", 2.0f, 0.5f, 50.0f);

    std::vector<Optics> opticsInStorage2 = { optics4, optics5, optics6 };

    Deliverer deliverer2("987654321", "Lens Suppliers Ltd.", "Plovdiv, Bulgaria", 
                          "+359 87 765 4321", opticsInStorage2);

    orderSystem.addDeliverer(deliverer1);
    orderSystem.addDeliverer(deliverer2);
}

void ApplicationStarter::mainMenu(InputHandler& ioHandler, OrderSystem& orderSystem)
{
    testInput(orderSystem);

    while(true) 
    {
        std::cout << "1. Add Deliverer" << std::endl;
        std::cout << "2. Add Optics To Deliverer" << std::endl;
        std::cout << "3. Place Order" << std::endl;
        std::cout << "4. Exit" << std::endl;
        std::cout << "Choose one of the options: ";
        int choice;
        std::cin >> choice;
        std::cout << std::endl;

        switch(choice)
        {
            case ADD_DELIVERER:
            {
                ioHandler.inputNewDeliverer(orderSystem);
                break;
            }
            case ADD_OPTICS_TO_DELIVERER:
            {   
                ioHandler.inputOpticsToDeliverer(orderSystem);
                break;
            }
            case PLACE_ORDER:
            {
                orderSystem.placeOrder();
                break;
            }
            case EXIT:
            {
                std::cout << "You exited the application!" << std::endl;
                return;        
            }
        }
    }
}

void ApplicationStarter::start()
{
    InputHandler inputHandler;
    OrderSystem orderSystem;

    this->mainMenu(inputHandler, orderSystem);
}