#pragma once

#include <chrono>

typedef unsigned long long timestamp;

class Timer {
	
public:
	
	Timer() :
	m_begin_time(since_epoch())
	{ }
	
	inline timestamp current() const {
		return since_epoch() - m_begin_time;
	}
	
	inline timestamp since_epoch() const {
		return std::chrono::time_point_cast <std::chrono::microseconds>
		(std::chrono::high_resolution_clock::now()).time_since_epoch().count();
	}
	
private:
	
	timestamp m_begin_time;
	
};
