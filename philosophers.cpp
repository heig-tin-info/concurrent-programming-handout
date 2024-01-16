#include <chrono>
#include <iostream>
#include <mutex>
#include <random>
#include <semaphore>
#include <thread>
#include <vector>

constexpr const size_t N = 5;

enum class State { THINKING = 0, HUNGRY = 1, EATING = 2, };

class SafePrint {
public:
    SafePrint(std::ostream& os) : os(os) {}

    template <typename T>
    SafePrint& operator<<(const T& t) {
        std::lock_guard<std::mutex> lk(m);
        os << t;
        return *this;
    }
};

SafePrint safe_cout(std::cout);

// Id of the left and right philosopher to a given philosopher
size_t inline left(size_t id) { return (id - 1 + N) % N; }
size_t inline right(size_t id) { return (id + 1) % N; }

State state[N];

std::mutex critical_section_mutex;
std::mutex output_mutex;

std::binary_semaphore both_forks_available[N];

size_t rand_range(size_t min, size_t max)
{
    static std::mt19937 rnd(std::time(nullptr));
    return std::uniform_int_distribution<>(min, max)(rnd);
}

void test(size_t i) {
    if (state[i] == State::HUNGRY &&
        state[left(i)] != State::EATING &&
        state[right(i)] != State::EATING)
    {
        state[i] = State::EATING;
        both_forks_available[i].release(); // forks are no longer needed for this eat session
    }
}

void think(size_t i)
{
    size_t duration = rand_range(400, 800);
    safe_cout << i << " is thinking " << duration << "ms" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(duration));
}

void take_forks(size_t i) {
    {
        std::lock_guard<std::mutex> lk{critical_section_mutex};
        state[i] = State::HUNGRY;
        safe_cout << "\t\t" << i << " is State::HUNGRY" << std::endl;
        test(i);
    }
    both_forks_available[i].acquire();
}

void eat(size_t i) {
    size_t duration = rand_range(400, 800);
    safe_cout << "\t\t\t\t" << i << " is eating " << duration << "ms"  << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(duration));
}

void put_forks(size_t id) {
    std::lock_guard<std::mutex> lk{critical_section_mutex};
    state[id] = State::THINKING;
    test(left(id));
    test(right(id));
}

void philosopher(size_t id) {
    for (;;) {
        think(id);
        take_forks(id);
        eat(id);
        put_forks(id);
    }
}

int main() {
    std::vector<std::jthread> philosophers;
    for (size_t i = 0; i < N; ++i) {
        both_forks_available[i].release(); // initially, both forks are available
        philosophers.emplace_back(philosopher, i);
    }
}