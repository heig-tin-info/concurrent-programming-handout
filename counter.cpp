#include <thread>
#include <iostream>
#include <chrono>

int counter = 0;

void thread() {
    for(;;) {
        counter++;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main() {
    std::jthread t1{thread};
    std::jthread t2{thread};
    while (true) {
        std::cout << counter << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}