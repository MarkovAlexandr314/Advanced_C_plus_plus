#include <iostream>
#include <chrono>
#include <queue>

#ifndef LOGH
#define LOGH

enum class LogLevel
{
    LOG_NORMAL,
    LOG_WARNING,
    LOG_ERROR
};

class Log
{

public:
    struct LogMessange
    {
        std::time_t time;
        std::string mgs;
        LogLevel lvl;
    };
    static Log *Instance()
    {
        if (!loger_prt)
            loger_prt = new Log();
        return loger_prt;
    }
    void message(LogLevel, std::string);
    void print();

private:
    static Log *loger_prt;
    std::queue<LogMessange> msg_queue;
    size_t msg_capacity{10};
    Log() {}
    Log(const Log &) = delete;
    Log &operator=(const Log &) = delete;
    std::time_t get_time()
    {
        auto now = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);
        return now_c;
    }
};

Log *Log::loger_prt = nullptr;

std::ostream &operator<<(std::ostream &stream, const Log::LogMessange &lm)
{
    switch (lm.lvl)
    {
    case (LogLevel::LOG_NORMAL):
        stream << "NORMAL:  ";
        break;
    case (LogLevel::LOG_WARNING):
        stream << "WARNING: ";
        break;
    case (LogLevel::LOG_ERROR):
        stream << "ERROR:   ";
        break;
    }
    char timedisplay[100];
    struct tm buf;
    errno_t err = localtime_s(&buf, &lm.time);
    if (std::strftime(timedisplay, sizeof(timedisplay), "%H:%M:%S", &buf))
    {
        stream << timedisplay << ' ';
    }
    stream << lm.mgs << '\n';
    return stream;
}

void Log::message(LogLevel lvl, std::string msg)
{
    if (msg_queue.size() < msg_capacity)
    {
        // размер очереди не превысил msg_capacity
        auto time = get_time();
        LogMessange lm{time, msg, lvl};
        msg_queue.push(lm);
    }
    else
    {
        // размер очереди превысил msg_capacity
        // удаляем самое старое сообщение
        msg_queue.pop();
        // добавляем новое
        auto time = get_time();
        LogMessange lm{time, msg, lvl};
        msg_queue.push(lm);
    }
}

void Log::print()
{
    while (!msg_queue.empty())
    {
        std::cout << msg_queue.front();
        msg_queue.pop();
    }
}
#endif // LOGH