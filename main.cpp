#include <iostream>
#include <thread>
#include <random>
#include <mutex>
#include <condition_variable>
#include <queue>
#include "config.hh"

struct SharedData {
    std::queue<uint64_t> values;
    std::mutex mutex;
    std::condition_variable cv;
    bool unitBReady = false; // Flag to signal when Unit B is ready
    bool unitCReady = false; // Flag to signal when Unit C is ready
    bool unitDReady = false; // Flag to signal when Unit D is ready

    void setValue(uint64_t val) {
        std::lock_guard<std::mutex> lock(mutex);
        values.push(val);
        cv.notify_one();
    }

    uint64_t getValue() {
        std::unique_lock<std::mutex> lock(mutex);
        cv.wait(lock, [this] { return !values.empty(); });
        uint64_t val = values.front();
        values.pop();
        return val;
    }

    bool isUnitBReady() {
        return unitBReady;
    }

    void setUnitBReady(bool ready) {
        unitBReady = ready;
    }

    bool isUnitCReady() {
        return unitCReady;
    }

    void setUnitCReady(bool ready) {
        unitCReady = ready;
    }

    bool isUnitDReady() {
        return unitDReady;
    }

    void setUnitDReady(bool ready) {
        unitDReady = ready;
    }
};

// Function to generate random numbers
uint64_t generateRandomNumber() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<uint64_t> dis(1, 1000);
    return dis(gen);
}

void unitA(SharedData& data) {
    while (true) {
        uint64_t num = generateRandomNumber();
        std::cout << "Unit A generated: " << num << std::endl;
        data.setValue(num);

        // Signal that Unit B is ready after generating the first value
        if (!data.isUnitBReady()) {
            data.setUnitBReady(true);
            data.cv.notify_one(); // Notify waiting threads
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void unitB(SharedData& data, uint64_t DIV) {
    {
        std::unique_lock<std::mutex> lock(data.mutex);
        data.cv.wait(lock, [&data] { return data.isUnitBReady(); }); // Wait until Unit B is ready
    }

    while (true) {
        uint64_t num = data.getValue();
        std::cout << "Unit B received: " << num << std::endl;
        num /= DIV;
        std::cout << "Unit B processed: " << num << std::endl;
        data.setValue(num);

        // Signal that Unit C is ready after processing the first value
        if (!data.isUnitCReady()) {
            data.setUnitCReady(true);
            data.cv.notify_one(); // Notify waiting threads
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void unitC(SharedData& data, uint64_t ADD) {
    {
        std::unique_lock<std::mutex> lock(data.mutex);
        data.cv.wait(lock, [&data] { return data.isUnitCReady(); }); // Wait until Unit C is ready
    }

    while (true) {
        uint64_t num = data.getValue();
        std::cout << "Unit C received: " << num << std::endl;
        num += ADD;
        std::cout << "Unit C processed: " << num << std::endl;
        data.setValue(num);

        // Signal that Unit D is ready after processing the first value
        if (!data.isUnitDReady()) {
            data.setUnitDReady(true);
            data.cv.notify_one(); // Notify waiting threads
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void unitD(SharedData& data) {
    {
        std::unique_lock<std::mutex> lock(data.mutex);
        data.cv.wait(lock, [&data] { return data.isUnitDReady(); }); // Wait until Unit D is ready
    }

    while (true) {
        uint64_t num = data.getValue();
        std::cout << "Unit D received: " << num << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Simulate processing time
    }
}

int main() {
    SharedData data;

    std::thread tA(unitA, std::ref(data));
    std::thread tB(unitB, std::ref(data), CONFIG.DIV);
    std::thread tC(unitC, std::ref(data), CONFIG.ADD);
    std::thread tD(unitD, std::ref(data));

    std::cout << "Main thread: All worker threads started" << std::endl;

    // Signal that all units are ready to start
    data.setUnitBReady(true);
    data.setUnitCReady(true);
    data.setUnitDReady(true);
    data.cv.notify_all();

    // Wait for all threads to finish
    tA.join();
    tB.join();
    tC.join();
    tD.join();

    std::cout << "Main thread: All worker threads finished" << std::endl;

    return 0;
}
