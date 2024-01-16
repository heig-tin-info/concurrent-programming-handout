#include <iostream>
#include <mutex>
#include <semaphore>
#include <thread>
#include <vector>
#include <memory>
#include <random>

size_t rand_range(size_t min, size_t max)
{
    static std::mt19937 rnd(std::time(nullptr));
    return std::uniform_int_distribution<>(min, max)(rnd);
}

class Philosopher {
public:
    Philosopher(int id, std::binary_semaphore& left_fork, std::binary_semaphore& right_fork)
        : id(id), left_fork(left_fork), right_fork(right_fork) {}

    void dine() {
        while (true) {
            think();
            eat();
        }
    }

private:
    int id;
    std::binary_semaphore& left_fork;
    std::binary_semaphore& right_fork;
    static std::mutex cout_mutex;

    void think() {
        {
            std::lock_guard<std::mutex> guard(cout_mutex);
            std::cout << "Philosopher " << id << " is thinking." << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(rand_range(400,1500)));
    }

    void eat() {
        left_fork.acquire();
        right_fork.acquire();
        {
            std::lock_guard<std::mutex> guard(cout_mutex);
            std::cout << "Philosopher " << id << " is eating." << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(rand_range(400,1500)));
        left_fork.release();
        right_fork.release();
    }
};

std::mutex Philosopher::cout_mutex;

int main() {
    const int num_philosophers = 5;
    std::vector<std::unique_ptr<std::binary_semaphore>> forks;
    for (int i = 0; i < num_philosophers; ++i) {
        forks.emplace_back(std::make_unique<std::binary_semaphore>(1));
    }

    std::vector<std::jthread> philosophers;
    philosophers.reserve(num_philosophers);

    for (int i = 0; i < num_philosophers; ++i) {
        philosophers.emplace_back(&Philosopher::dine, Philosopher(i, *forks[i], *forks[(i + 1) % num_philosophers]));
    }

    // Pas besoin d'appeler join(), car std::jthread le fait automatiquement.
    return 0;
}
