#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <vector>

#ifdef _OPENMP
#include <omp.h>
#endif

namespace Clock = std::chrono;

std::vector<double> generate_random_array(size_t size) {
    std::vector<double>                    arr(size);
    std::mt19937                           gen(std::random_device{}());
    std::uniform_real_distribution<double> dist(0.0, 100.0);

    for (size_t i = 0; i < size; ++i) {
        arr[i] = dist(gen);
    }

    return arr;
}

void sequential_sin_transform(const std::vector<double> &input, std::vector<double> &output) {
    for (size_t i = 0; i < input.size(); ++i) {
        output[i] = std::sin(input[i]);
    }
}

void sin_chunk_worker(const std::vector<double> &input,
                      std::vector<double>       &output,
                      size_t                     start_idx,
                      size_t                     end_idx) {
    for (size_t i = start_idx; i < end_idx; ++i) {
        output[i] = std::sin(input[i]);
    }
}

void parse_command_line(int argc, char *argv[], size_t &array_size, int &specific_thread_count) {
    if (argc >= 2) {
        array_size = std::stoull(argv[1]);
        if (array_size == 0) {
            std::cerr << "Array size must be > 0!\n";
            std::exit(1);
        }
    }

    if (argc >= 3) {
        specific_thread_count = std::stoi(argv[2]);
        if (specific_thread_count <= 0) {
            std::cerr << "Thread count must be > 0!\n";
            std::exit(1);
        }
    }
}

Clock::milliseconds parallel_sin_transform_static(const std::vector<double> &input,
                                                  std::vector<double>       &output,
                                                  int                        num_threads) {

    auto                     start_time = Clock::steady_clock::now();
    std::vector<std::thread> threads;
    size_t                   input_size = input.size();
    size_t                   chunk_size = input_size / num_threads;

    for (size_t i = 0; i < (size_t)num_threads; ++i) {
        size_t start_idx = i * chunk_size;
        size_t end_idx = (i == (size_t)num_threads - 1) ? input_size : (start_idx + chunk_size);

        threads.emplace_back(sin_chunk_worker, std::cref(input), std::ref(output), start_idx,
                             end_idx);
    }

    for (auto &t : threads) {
        t.join();
    }

    auto end_time = Clock::steady_clock::now();
    return Clock::duration_cast<Clock::milliseconds>(end_time - start_time);
}

Clock::milliseconds parallel_sin_transform_omp([[maybe_unused]] const std::vector<double> &input,
                                               [[maybe_unused]] std::vector<double>       &output,
                                               [[maybe_unused]] int num_threads) {
#ifdef _OPENMP
    omp_set_num_threads(num_threads);
    auto start_time = Clock::steady_clock::now();

#pragma omp parallel for
    for (size_t i = 0; i < input.size(); ++i) {
        output[i] = std::sin(input[i]);
    }

    auto end_time = Clock::steady_clock::now();
    return Clock::duration_cast<Clock::milliseconds>(end_time - start_time);
#else
    return Clock::milliseconds(0);
#endif
}

bool check_correctness(const std::vector<double> &a,
                       const std::vector<double> &b,
                       double                     tolerance = 1e-6) {
    if (a.size() != b.size()) {
        return false;
    }

    for (size_t i = 0; i < a.size(); ++i) {
        if (std::fabs(a[i] - b[i]) > tolerance) {
            return false;
        }
    }
    return true;
}

int main(int argc, char *argv[]) {
    size_t array_size = 10000000;
    int    specific_thread_count = -1;

    parse_command_line(argc, argv, array_size, specific_thread_count);

    std::cout << "Array size: " << array_size << "\n";

    auto                data = generate_random_array(array_size);
    std::vector<double> result_seq(array_size);
    std::vector<double> result_par(array_size);

    auto t_seq_start = Clock::steady_clock::now();

    sequential_sin_transform(data, result_seq);

    auto t_seq_end = Clock::steady_clock::now();
    auto seq_time = Clock::duration_cast<Clock::milliseconds>(t_seq_end - t_seq_start).count();

    std::cout << "Sequential time: " << seq_time << " ms\n\n";

    std::vector<int> thread_counts = {1, 2, 4, 8};

    if (specific_thread_count > 0) {
        thread_counts = {specific_thread_count};
    }

    std::cout << "=== STD::THREAD RESULTS ===\n";
    std::cout << "Threads | Time (ms) | Speedup | Efficiency (%) | Correct?\n";
    std::cout << "------------------------------------------------------------\n";

    for (int num_threads : thread_counts) {
        auto   duration = parallel_sin_transform_static(data, result_par, num_threads);
        double speedup = static_cast<double>(seq_time) / duration.count();
        double efficiency = (speedup / num_threads) * 100.0;
        bool   correct = check_correctness(result_seq, result_par);

        std::cout << std::setw(7) << num_threads << " | " << std::setw(9) << duration.count()
                  << " | " << std::setw(7) << std::fixed << std::setprecision(2) << speedup << " | "
                  << std::setw(14) << std::fixed << std::setprecision(2) << efficiency << " | "
                  << (correct ? "Yes" : "No") << "\n";
    }

#ifdef _OPENMP
    std::cout << "\n=== OPENMP RESULTS ===\n";
    std::cout << "Threads | Time (ms) | Speedup | Efficiency (%) | Correct?\n";
    std::cout << "------------------------------------------------------------\n";

    for (int num_threads : thread_counts) {
        auto   duration = parallel_sin_transform_omp(data, result_par, num_threads);
        double speedup = static_cast<double>(seq_time) / duration.count();
        double efficiency = (speedup / num_threads) * 100.0;
        bool   correct = check_correctness(result_seq, result_par);

        std::cout << std::setw(7) << num_threads << " | " << std::setw(9) << duration.count()
                  << " | " << std::setw(7) << std::fixed << std::setprecision(2) << speedup << " | "
                  << std::setw(14) << std::fixed << std::setprecision(2) << efficiency << " | "
                  << (correct ? "Yes" : "No") << "\n";
    }
#endif

std::cout << std::thread::hardware_concurrency();
    return 0;

}
