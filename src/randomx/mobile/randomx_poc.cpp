
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <numeric>

// Simulate RandomX hashing work
void simulate_randomx_work(int iterations) {
    volatile long long dummy_sum = 0;
    for (int i = 0; i < iterations; ++i) {
        dummy_sum += i * i;
    }
}

// Function to simulate CPU throttling to a target percentage
void throttled_work(int work_duration_ms, double target_cpu_percent) {
    if (target_cpu_percent <= 0 || target_cpu_percent > 100) {
        std::cerr << "Invalid target_cpu_percent. Must be between 0 and 100." << std::endl;
        return;
    }

    // Calculate active and sleep times based on target_cpu_percent
    // For example, if target_cpu_percent is 10%, then 10% active, 90% sleep
    double active_ratio = target_cpu_percent / 100.0;
    double sleep_ratio = 1.0 - active_ratio;

    long long active_time_us = static_cast<long long>(work_duration_ms * active_ratio * 1000);
    long long sleep_time_us = static_cast<long long>(work_duration_ms * sleep_ratio * 1000);

    auto start_time = std::chrono::high_resolution_clock::now();

    while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start_time).count() < work_duration_ms) {
        // Simulate active work for a short period
        simulate_randomx_work(10000); // Adjust iterations for desired work intensity

        // Sleep for the calculated duration to throttle CPU
        if (sleep_time_us > 0) {
            std::this_thread::sleep_for(std::chrono::microseconds(sleep_time_us / 10)); // Sleep in smaller chunks
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " <work_duration_ms> <target_cpu_percent>" << std::endl;
        return 1;
    }

    int work_duration_ms = std::stoi(argv[1]);
    double target_cpu_percent = std::stod(argv[2]);

    std::cout << "Starting RandomX PoC simulation with work duration: " << work_duration_ms << "ms, target CPU: " << target_cpu_percent << "%" << std::endl;

    auto start_total = std::chrono::high_resolution_clock::now();
    throttled_work(work_duration_ms, target_cpu_percent);
    auto end_total = std::chrono::high_resolution_clock::now();

    auto duration_total = std::chrono::duration_cast<std::chrono::milliseconds>(end_total - start_total).count();
    std::cout << "Simulation finished in " << duration_total << "ms." << std::endl;

    return 0;
}
