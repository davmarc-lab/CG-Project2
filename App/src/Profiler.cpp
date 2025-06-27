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

void Profiler::dump(const std::string &msg) const {
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

void Profiler::dump(double &wall, double &cpu) const {
	wall = this->m_wall;
	cpu = this->m_cpu;
}

void Profiler::wallDump() const {
	std::cout << "Wall Time: " << this->m_wall << "s\n";
}

void Profiler::cpuDump() const {
	std::cout << "CPU Time: " << this->m_cpu << "s\n";
}

std::string Profiler::getProfilerFilter() const {
	return filterToString(this->m_profilerFilter);
}

#ifdef _WIN32	// Windows implementation
void WProfiler::start() {
	this->m_start = std::clock();
}

void WProfiler::end() {
	this->m_end = std::clock();
	this->m_wall = static_cast<double>(this->m_end - this->m_start) / CLOCKS_PER_SEC;
}
#else // Linux implementation
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
void LinuxProfiler::start() { getTime(this->m_profilerFilter, this->m_wallBegin, this->m_cpuBegin); }

void LinuxProfiler::end() {
	getTime(this->m_profilerFilter, this->m_wallEnd, this->m_cpuEnd);
	this->m_wall = this->m_wallEnd.tv_nsec - this->m_wallBegin.tv_nsec;
	this->m_wall *= 1e-9;
	
	this->m_cpu = this->m_cpuEnd.tv_nsec - this->m_cpuBegin.tv_nsec;
	this->m_cpu *= 1e-9;
}
#endif
