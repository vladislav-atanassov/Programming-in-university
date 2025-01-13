#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <chrono>

std::mutex mtx;
// Could be implemented with C style array, std::array, std::vector...
std::queue<int> swimmers_queue;  // Queue of swimmers time
constexpr int TOTAL_SWIMMERS = 7;
int total_time = 0;

void swimming_lane() 
{
    int lane_time = 0;

    while(true) 
    {
        int swimmer_time;

        {
            std::lock_guard<std::mutex> lock(mtx);

            if(swimmers_queue.empty()) 
            {
                break;
            }
            
            // The next swimmer gets in
            swimmer_time = swimmers_queue.front();
            swimmers_queue.pop();
        }

        // Update the current lane time
        lane_time += swimmer_time;

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::lock_guard<std::mutex> lock(mtx);

    // Update total time after all swimmers in this lane finish
    if(lane_time > total_time) 
    {
        total_time = lane_time;
    }
}

int main() 
{
    for(int i = 1; i <= TOTAL_SWIMMERS; i++) 
    {
        swimmers_queue.push(i * 6); // swimmers swim time)
    }

    std::thread lane1(swimming_lane);
    std::thread lane2(swimming_lane);
    std::thread lane3(swimming_lane);
    std::thread lane4(swimming_lane);

    lane1.join();
    lane2.join();
    lane3.join();
    lane4.join();

    std::cout << "Total time for all swimmers to swim once: " << total_time << " minutes.\n";

    return 0;
}
