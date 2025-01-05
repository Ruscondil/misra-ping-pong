#ifndef MISRA_H
#define MISRA_H

#include <iostream>
#include <cmath>
#include <thread>
#include <mutex>
#include <chrono>
#include <random>
#include "sender.h"

enum ping_state
{
    PING_OUTSIDE,
    PING_INSIDE,
    IN_SECTION,
};

class Worker
{
public:
    Worker();
    void start();
    void enterCriticalSection();

private:
    void run();
    std::thread worker_thread;
    std::mutex mtx;
    bool stop;
};

class Misra
{
public:
    Misra(Sender &sender);
    void regenerate(int64_t &x);
    void incarnate(int64_t &x);
    void process(int64_t number);

private:
    int64_t mi;
    int64_t ping;
    int64_t pong;
    Sender &sender;
    ping_state ping_state_;
    Worker worker;
};

#endif // MISRA_H
