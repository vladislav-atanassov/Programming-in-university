#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;

int operating_memory = 4;
constexpr int MEMORY_FOR_TASK = 2;
constexpr int NUMBER_OF_TASKS = 4;

size_t next_task_index = 0;  // The index of the next task to run in the sequence
std::vector<std::pair<std::thread, int>> tasks;

void task(int task_number)
{
    while (true)
    {
        {
            std::unique_lock<std::mutex> lock(mtx);

            // Wait for the task's turn and for sufficient memory to be available
            cv.wait(lock, [task_number]()
            {
                return operating_memory >= MEMORY_FOR_TASK &&
                       tasks[next_task_index].second == task_number;
            });

            // Perform the task
            std::cout << "Task " << task_number << " working" << std::endl;

            // Consume memory
            operating_memory -= MEMORY_FOR_TASK;

            // Move to the next task in the sequence
            next_task_index = (next_task_index + 1) % tasks.size();
        }

        std::this_thread::sleep_for(std::chrono::seconds(2));

        {
            std::lock_guard<std::mutex> lock(mtx);

            // Release memory and notify the next waiting task
            operating_memory += MEMORY_FOR_TASK;
            cv.notify_all();
        }
    }
}

int main()
{
    // Add even tasks
    for(size_t i = 1; i <= NUMBER_OF_TASKS; i++) 
    {
        if(i % 2 == 0) 
        {
            tasks.emplace_back(std::thread(task, i), i);
        }
    }

    // Add odd tasks
    for(size_t i = 1; i <= NUMBER_OF_TASKS; i++) 
    {
        if(i % 2 != 0) 
        {
            tasks.emplace_back(std::thread(task, i), i);
        }
    }

    for(auto& task : tasks) 
    {
        task.first.join();
    }

    return 0;
}
