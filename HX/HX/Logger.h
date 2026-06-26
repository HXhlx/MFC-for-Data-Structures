#pragma once
#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>
#include <mutex>
#include <chrono>
#include <iomanip>
#include <sstream>

class Logger
{
public:
    static Logger& Instance()
    {
        static Logger instance;
        return instance;
    }

    void Log(const std::string& message)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_file.is_open())
        {
            m_file << GetTimestamp() << " " << message << std::endl;
            m_file.flush();
        }
    }

private:
    Logger()
    {
        m_file.open("hx_debug.log", std::ios::out | std::ios::trunc);
        if (m_file.is_open())
        {
            m_file << "=== HX Debug Log Started ===" << std::endl;
        }
    }

    ~Logger()
    {
        if (m_file.is_open())
        {
            m_file << "=== HX Debug Log Ended ===" << std::endl;
            m_file.close();
        }
    }

    std::string GetTimestamp()
    {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::tm tm;
        localtime_s(&tm, &time);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%H:%M:%S");
        return oss.str();
    }

    std::ofstream m_file;
    std::mutex m_mutex;
};

#define LOG(msg) Logger::Instance().Log(msg)

#endif // LOGGER_H
