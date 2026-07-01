#pragma once
#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>
#include <mutex>
#include <chrono>
#include <format>

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
        return std::format("{:%H:%M:%S}", std::chrono::system_clock::now());
    }

    std::ofstream m_file;
    std::mutex m_mutex;
};

#define LOG(msg) Logger::Instance().Log(msg)

#endif // LOGGER_H
