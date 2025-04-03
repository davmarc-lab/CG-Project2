#pragma once

#include <ctime>
#include <string>

enum ProfileFilter {
	PROFILE_WALL,
	PROFILE_CPU,
	PROFILE_ALL,
};

class Profiler {
public:
	virtual void start();
	virtual void end();

	void dump(const std::string &msg = "");
	void wallDump();
	void cpuDump();

	std::string getProfilerFilter() const;
	inline void setProfilerFilter(const ProfileFilter &filter) { this->m_profilerFilter = filter; }

	inline double getLastWall() const { return this->m_wall; }
	inline double getLastCPU() const { return this->m_cpu; }

	Profiler(const ProfileFilter &filter = PROFILE_ALL) :
		m_profilerFilter(filter) {}

private:
	ProfileFilter m_profilerFilter{PROFILE_ALL};

	std::timespec m_wallBegin{};
	std::timespec m_wallEnd{};
	std::timespec m_cpuBegin{};
	std::timespec m_cpuEnd{};

	double m_wall{};
	double m_cpu{};
};
