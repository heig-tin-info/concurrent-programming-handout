#include <iostream>
#include <future>
#include <thread>
#include <chrono>

int snailTask(int x) {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return x * 2;
}

int slothTask(int y) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return y + 3;
}

int main() {
    std::future<int> result1 = std::async(std::launch::async, longRunningTask, 5);
    int output1 = result1.get();
    std::future<int> result2 = std::async(std::launch::async, slothTask, output1);
    int output2 = result2.get();
    std::cout << output2 << std::endl;
}