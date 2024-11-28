#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

constexpr int STOP_CREDITING_THRESHOLD = 5000;

std::mutex mutex;
int bank_balance = 10000;

void bank_operations(short customer_id)
{
    int customer_balance = 0;
    int customer_waiting_period = 0;

    while(true)
    {
        mutex.lock();

        if(customer_waiting_period == 0)
        {
            if(bank_balance >= 500)
            {
                bank_balance -= 250;
                customer_balance += 250;
            }

            if(customer_balance >= STOP_CREDITING_THRESHOLD)
            {
                customer_waiting_period = 6;
            }
        }
        else
        {
            customer_balance -= 250;
            bank_balance += 250;

            --customer_waiting_period;
        }

        std::cout << "Current bank balance: " << bank_balance 
                << ", Current customer " << customer_id << " balance: " << customer_balance << std::endl;

        mutex.unlock();
    
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main()
{
    std::thread customer1(bank_operations, 111);
    std::thread customer2(bank_operations, 222);

    customer1.join();
    customer2.join();
}