#include "../include/Profiler.hpp"

#include <ctime>
#include <iostream>
#include <sstream>
#include <string>

std::string filterToString(const ProfileFilter &filter) {
	switch (filter) {
		case PROFILE_WALL:
			return "Wall";
		case PROFILE_CPU:
			return "CPU";
		case PROFILE_ALL:
			return "Wall + CPU";
	}
	return "";
}

void getTime(ProfileFilter &filter, std::timespec &wallTime, std::timespec &cpuTime) {
	switch (filter) {
		case PROFILE_WALL: {
			clock_gettime(CLOCK_REALTIME, &wallTime);
			return;
		}
		case PROFILE_CPU: {
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &cpuTime);
			return;
		}
		case PROFILE_ALL: {
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &cpuTime);
			clock_gettime(CLOCK_REALTIME, &wallTime);
			return;
		}
	}
}

void Profiler::start() { getTime(this->m_profilerFilter, this->m_wallBegin, this->m_cpuBegin); }

void Profiler::end() { getTime(this->m_profilerFilter, this->m_wallEnd, this->m_cpuEnd); }

void Profiler::dump(const std::string &msg) {
	this->m_wall = this->m_wallEnd.tv_nsec - this->m_wallBegin.tv_nsec;
	this->m_wall *= 1e-9;

	this->m_cpu = this->m_cpuEnd.tv_nsec - this->m_cpuBegin.tv_nsec;
	this->m_cpu *= 1e-9;

	if (!msg.empty()) {
		std::cout << msg << "\n";
	}

	switch (this->m_profilerFilter) {
		case PROFILE_WALL: {
			this->wallDump();
			return;
		}
		case PROFILE_CPU: {
			this->cpuDump();
			return;
		}
		case PROFILE_ALL: {
			this->wallDump();
			this->cpuDump();
			return;
		}
	}
}

void Profiler::wallDump() {
	std::cout << "Wall Time: " << this->m_wall << "s\n";
}

void Profiler::cpuDump() {
	std::cout << "CPU Time: " << this->m_cpu << "s\n";
}

std::string Profiler::getProfilerFilter() const {
	return filterToString(this->m_profilerFilter);
}
