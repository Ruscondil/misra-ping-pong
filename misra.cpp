#include "misra.h"

Misra::Misra(Sender &sender) : sender(sender), mi(0), ping(1), pong(-ping), ping_state_(PING_OUTSIDE), stop(false), worker_done(false), work_available(false)
{
    startWorker();
}

Misra::~Misra()
{
    {
        std::lock_guard<std::mutex> lock(mtx);
        stop = true;
        work_available = true;
    }
    cv.notify_one();
    if (worker_thread.joinable())
    {
        worker_thread.join();
    }
}

void Misra::startWorker()
{
    worker_thread = std::thread(&Misra::runWorker, this);
}

void Misra::notifyWorker()
{
    {
        std::lock_guard<std::mutex> lock(mtx);
        work_available = true;
    }
    cv.notify_one();
}

void Misra::runWorker()
{
    while (true)
    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this]
                { return work_available || stop; });

        if (stop)
        {
            break;
        }

        work_available = false;
        lock.unlock();

        lockPing();

        // Generate a random duration between 0.5 and 3 seconds
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.5, 3.0);
        double duration = dis(gen);

        // Perform work for the random duration
        std::this_thread::sleep_for(std::chrono::duration<double>(duration));
        std::cout << "Exiting critical section after " << duration << " seconds" << std::endl;

        unlockPing();
    }
}

void Misra::regenerate(int64_t &x)
{
    ping = std::abs(x);
    pong = -ping;
    std::cout << "Regenerated: ping = " << ping << ", pong = " << pong << std::endl;
}

void Misra::incarnate(int64_t &x)
{
    ping = std::abs(x) + 1;
    pong = -ping;
    // std::cout << "Incarnated: ping = " << ping << ", pong = " << pong << std::endl;
}

void Misra::process(int64_t number)
{
    if (number > 0)
    { // PING
        std::cout << "Received PING: " << number << std::endl;
        if (mi == number)
        {
            regenerate(ping);
            sender.send(pong);
            // std::cout << "Sent PONG: " << pong << std::endl;
        }
        else
        {
            ping = number;
        }

        notifyWorker();
    }
    else
    { // PONG
        // std::cout << "Received PONG: " << number << std::endl;
        if (ping_state_ == IN_SECTION)
        {
            incarnate(pong);
        }
        else
        {
            if (mi == number)
            {
                regenerate(pong);
                sender.send(ping);
                // std::cout << "Sent PING: " << ping << std::endl;
                mi = pong;
            }
            else
            {
                pong = number;
                mi = pong;
            }
        }
        sender.send(pong);
        // std::cout << "Sent PONG: " << pong << std::endl;
    }
}

void Misra::lockPing()
{
    std::lock_guard<std::mutex> lock(mtx);
    ping_state_ = IN_SECTION;
    std::cout << "Acquired PING, state changed to IN_SECTION" << std::endl;
}

void Misra::unlockPing()
{
    std::lock_guard<std::mutex> lock(mtx);
    sender.send(ping);
    mi = ping;
    ping_state_ = PING_OUTSIDE;
    std::cout << "Released PING, state changed to PING_OUTSIDE" << std::endl;
}
