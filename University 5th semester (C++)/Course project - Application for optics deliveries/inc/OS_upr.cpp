#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

std::mutex mtx;
int currCapacity = 0;
constexpr int poolMaxCapacity = 5000;

void fill_pool(int addedWater, int timeForFilling)
{
    bool sleepFlag = false;

    while(true)
    {
        mtx.lock();

        if(((currCapacity + addedWater) > poolMaxCapacity) || ((currCapacity + addedWater) < 0))
        {
            sleepFlag = true;
        }
        else
        {
            currCapacity += addedWater;
        }

        mtx.unlock();
    
        if(sleepFlag)
        {
            std::this_thread::sleep_for(std::chrono::seconds(10));
            sleepFlag = false;
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(timeForFilling));
        }
    }
}

int main()
{
    int timeRunning = 0;

    std::thread *th1 = new std::thread(fill_pool, 10, 30);
    std::thread *th2 = new std::thread(fill_pool, 50, 70);
    std::thread *th3 = new std::thread(fill_pool, 300, 1000);

    while(true)
    {
        mtx.lock();

        std::cout << "Time: " << timeRunning << " Pool Capacity: " << currCapacity << std::endl;

        mtx.unlock();

        timeRunning++;

        std::this_thread::sleep_for(std::chrono::seconds(1)); 
    }

    th1->join();
    th2->join();
    th3->join();

    return 0;
}