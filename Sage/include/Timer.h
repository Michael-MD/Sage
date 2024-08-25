#pragma once

#include <chrono>
#include <string>
#include <iostream>

class Timer {
public:
    Timer(std::string tmp_title) {
        m_startpoint = std::chrono::high_resolution_clock::now();
        title = tmp_title;
    }

    ~Timer() {
        auto m_endpoint = std::chrono::high_resolution_clock::now();
        auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startpoint).time_since_epoch().count();
        auto end = std::chrono::time_point_cast<std::chrono::microseconds>(m_endpoint).time_since_epoch().count();
        auto duration = end - start;
        auto duration_s = duration * 1e-6;
        std::cout << title << ": " << duration << "us (" << duration_s << " sec)" << std::endl;
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_startpoint;
    std::string title;
};