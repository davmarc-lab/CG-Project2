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
	virtual void start() {}

	virtual void end() {}

	void wallDump() const;

	void cpuDump() const;

	void dump(const std::string &msg = "") const;
	
	void dump(double& wall, double& cpu) const;

	std::string getProfilerFilter() const;
	inline void setProfilerFilter(const ProfileFilter &filter) { this->m_profilerFilter = filter; }

	Profiler(const ProfileFilter &filter = PROFILE_ALL) :
		m_profilerFilter(filter) {
	}

	virtual ~Profiler() = default;

protected:
	ProfileFilter m_profilerFilter{PROFILE_ALL};

	double m_wall{};
	double m_cpu{};
};

#ifdef _WIN32

class WProfiler : public Profiler {
public:
	WProfiler(const ProfileFilter &filter = PROFILE_ALL) :
		Profiler(filter) {
	}

	virtual ~WProfiler() override = default;

	void start() override;
	void end() override;

	inline double getLastWall() const { return this->m_wall; }
	inline double getLastCPU() const { return this->m_cpu; }

private:
	std::time_t m_start{};
	std::time_t m_end{};
};

#else

class LinuxProfiler : public Profiler {
public:
	LinuxProfiler(const ProfileFilter &filter = PROFILE_ALL) :
		Profiler(filter) {
	}
	
	virtual ~LinuxProfiler() override = default;

	void start() override;
	void end() override;

	inline double getLastWall() const { return this->m_wall; }
	inline double getLastCPU() const { return this->m_cpu; }

private:
	std::timespec m_wallBegin{};
	std::timespec m_wallEnd{};
	std::timespec m_cpuBegin{};
	std::timespec m_cpuEnd{};
};
#endif


