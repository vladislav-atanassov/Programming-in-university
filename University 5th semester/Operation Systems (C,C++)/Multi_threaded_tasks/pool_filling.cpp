#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

std::mutex mutex;
int curr_capacity = 0;
constexpr int POOL_MAX_CAPACITY = 5000;

void fill_pool(int added_water, int timeForFilling)
{
    bool sleep_flag = false;

    while(true)
    {
        mutex.lock();

        if(((curr_capacity + added_water) > POOL_MAX_CAPACITY) || ((curr_capacity + added_water) < 0))
        {
            sleep_flag = true;
        }
        else
        {
            curr_capacity += added_water;
        }

        mutex.unlock();
    
        if(sleep_flag)
        {
            std::this_thread::sleep_for(std::chrono::seconds(10));
            sleep_flag = false;
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(timeForFilling));
        }
    }
}

int main()
{
    int time_running = 0;

    std::thread th1(fill_pool, 10, 30);
    std::thread th2(fill_pool, 50, 70);
    std::thread th3(fill_pool, 300, 1000);

    while(true)
    {
        mutex.lock();

        std::cout << "Time: " << time_running << " Pool Capacity: " << curr_capacity << std::endl;

        mutex.unlock();

        time_running++;

        std::this_thread::sleep_for(std::chrono::seconds(1)); 
    }

    th1.join();
    th2.join();
    th3.join();

    return 0;
}
