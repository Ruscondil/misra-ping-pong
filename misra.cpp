#include <iostream>
#include <cmath>
#include "sender.cpp"

enum ping_state
{
    NO_PING,
    HAVE_PING,
    IN_SECTION,
    PING_INCARNATED,
};

class Misra
{
public:
    Misra(Sender &sender) : sender(sender), mi(0), ping(1), pong(-ping), ping_state_(NO_PING) {}

    void regenerate(int64_t &x)
    {
        ping = std::abs(x);
        pong = -ping;
    }

    void incarnate(int64_t &x)
    {
        ping = std::abs(x) + 1;
        pong = -ping;
    }

    void process(int64_t number)
    {
        if (number > 0)
        { // PING
            if (mi == number)
            {
                regenerate(ping);
                sender.send(pong);
            }
            else
            {
                ping = number;
            }

            if (ping_state_ == NO_PING)
            {
                ping_state_ = HAVE_PING;
            }
        }
        else
        { // PONG
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
                    mi = pong;
                }
                else
                {
                    pong = number;
                    mi = pong;
                }
            }
            sender.send(pong);
        }
    }

private:
    int64_t mi;
    int64_t ping;
    int64_t pong;
    Sender &sender;
    ping_state ping_state_;
};
