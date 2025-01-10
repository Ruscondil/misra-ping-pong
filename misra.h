#ifndef MISRA_H
#define MISRA_H

#include <iostream>
#include <cmath>
#include <thread>
#include <mutex>
#include <chrono>
#include <random>
#include <condition_variable>
#include "sender.h"

enum ping_state
{
    PING_OUTSIDE,
    PING_INSIDE,
    IN_SECTION,
};

class Misra
{
public:
    Misra(Sender &sender);
    ~Misra();
    void regenerate(int64_t &x);
    void incarnate(int64_t &x);
    void process(int64_t number);
    void acquirePing();
    void releasePing();
    void startWorker();
    void notifyWorker();

private:
    void runWorker();
    int64_t mi;
    int64_t ping;
    int64_t pong;
    Sender &sender;
    ping_state ping_state_;
    std::thread worker_thread;
    std::mutex mtx;
    std::condition_variable cv;
    bool stop;
    bool worker_done;
    bool work_available;
};

#endif // MISRA_H
