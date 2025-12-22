#pragma once

#include <ctime>
#include <string>

#include "../../Opengl-Core/include/Core.hpp"

/**
 * @enum ProfileFilter
 * @brief Filters the type of time the profiler tracks.
 */
enum ProfileFilter {
	PROFILE_WALL,
	PROFILE_CPU,
	PROFILE_ALL,
};

/**
 * @brief The profiler is used to take cpu and/or wall times and
 * calculate the delta time between operations.
 */
class Profiler {
public:
	/**
	 * @brief Starts the timer, it should be used before Profiler::end().
	 */
	virtual void start() {}

	/**
	 * @brief Stops the timer, it should be used after Profiler::start().
	 */
	virtual void end() {}

	/**
	 * @brief Prints in the standard output the wall time.
	 */
	void wallDump() const;

	/**
	 * @brief Prints in the standard output the cpu time.
	 */
	void cpuDump() const;

	/**
	 * @brief Prints in the standard output wall and cpu time with
	 * the given message.
	 *
	 * @param msg the message before the times
	 */
	void dump(const std::string &msg = "") const;

	/**
	 * @brief Dumps the wall and cpu time in the given parameters.
	 *
	 * @param[in, out] wall the wall time
	 * @param[in, out] cpu the cpu time
	 */
	void dump(double &wall, double &cpu) const;

	/**
	 * @brief Retrieves the current profiler filter name.
	 *
	 * @return the name of the current profiler
	 */
	std::string getProfilerFilter() const;
	/**
	 * @brief Changes the current profiler filter.
	 *
	 * @param filter the profiler filter
	 */
	inline void setProfilerFilter(const ProfileFilter &filter) { this->m_profilerFilter = filter; }

	/**
	 * @brief Instances basic Profiler with the given filter or
	 * ProfilerFilter::PROFILE_ALL by deafult.
	 *
	 * @param filter the ProfilerFilter to be used
	 */
	Profiler(const ProfileFilter &filter = PROFILE_ALL) :
		m_profilerFilter(filter) {
	}

	/**
	 * @brief Basic destructor.
	 */
	virtual ~Profiler() = default;

protected:
	/// profiler filter for taking times
	ProfileFilter m_profilerFilter{PROFILE_ALL};

	/// wall time
	double m_wall{};
	/// cpu time
	double m_cpu{};
};

#ifdef _WIN32

/**
 * @brief Profiler implementation for Windows build.
 */
class WProfiler : public Profiler {
public:
	WProfiler(const ProfileFilter &filter) = delete;

	WProfiler(WProfiler &other) = delete;

	void operator=(const WProfiler &other) = delete;

	/**
	 * @brief Retrieves the instance of the WProfiler.
	 * If it's not instanced, it will be instanced automatically.
	 *
	 * @return a Shared<WProfiler> object
	 */
	inline static Shared<WProfiler> instance() {
		if (s_pointer == nullptr) {
			Shared<WProfiler> copy(new WProfiler());
			copy.swap(s_pointer);
		}
		return s_pointer;
	}

	virtual ~WProfiler() override = default;

	void start() override;
	void end() override;

    /**
     * @brief Retrieves the last wall time registered.
     *
     * @return the last registered wall time
     */
	inline double getLastWall() const { return this->m_wall; }
    /**
     * @brief Retrieves the last cpu time registered.
     *
     * @return the last registered cpu time
     */
	inline double getLastCPU() const { return this->m_cpu; }

	    /**
	 * @brief Dumps to the standard output the input times.
	 */
	inline void dumpInput() { this->dump(this->inputWalltime, this->inputCputime); }
	/**
	 * @brief Dumps to the standard output the update times.
	 */
	inline void dumpUpdate() { this->dump(this->updateWalltime, this->updateCputime); }
	/**
	 * @brief Dumps to the standard output the render times.
	 */
	inline void dumpRender() { this->dump(this->renderWalltime, this->renderCputime); }

    /**
	 * @brief Retrieves the input wall and cpu times in a Pair<double>.
	 *
	 * @return a pair containing wall and cpu times
	 */
	inline Pair<double> getInputTime() { return {this->inputWalltime, this->inputCputime}; }
	/**
	 * @brief Retrieves the update wall and cpu times in a Pair<double>.
	 *
	 * @return a pair containing wall and cpu times
	 */
	inline Pair<double> getUpdateTime() { return {this->updateWalltime, this->updateCputime}; }
	/**
	 * @brief Retrieves the render wall and cpu times in a Pair<double>.
	 *
	 * @return a pair containing wall and cpu times
	 */
	inline Pair<double> getRenderTime() { return {this->renderWalltime, this->renderCputime}; }

private:
	WProfiler() = default;

	/// static shared pointer for Singleton
	inline static Shared<WProfiler> s_pointer = nullptr;
    /// start time
	std::time_t m_start{};
    /// end time
	std::time_t m_end{};

	/// input wall time
	double inputWalltime{};
	/// input cpu time
	double inputCputime{};
	/// update wall time
	double updateWalltime{};
	/// update cpu time
	double updateCputime{};
	/// render wall time
	double renderWalltime{};
	/// render cpu time
	double renderCputime{};
};

#else

/**
 * @brief Profiler implementation for Linux build.
 */
class LinuxProfiler : public Profiler {
public:
	LinuxProfiler(const ProfileFilter &filter) = delete;

	LinuxProfiler(LinuxProfiler &other) = delete;

	void operator=(const LinuxProfiler &other) = delete;

	/**
	 * @brief Retrieves the instance of the LinuxProfiler.
	 * If it's not instanced, it will be instanced automatically.
	 *
	 * @return a Shared<LinuxProfiler> object
	 */
	inline static Shared<LinuxProfiler> instance() {
		if (s_pointer == nullptr) {
			Shared<LinuxProfiler> copy(new LinuxProfiler());
			copy.swap(s_pointer);
		}
		return s_pointer;
	}

	virtual ~LinuxProfiler() override = default;

	virtual void start() override;
	virtual void end() override;

	/**
	 * @brief Retrieves the last wall time recorded.
	 *
	 * @return the last wall time
	 */
	inline double getLastWall() const { return this->m_wall; }
	/**
	 * @brief Retrieves the last cpu time recorded.
	 *
	 * @return the last cpu time
	 */
	inline double getLastCPU() const { return this->m_cpu; }

    /**
    * @brief Dumps to the standard output the input times.
    */
	inline void dumpInput() { this->dump(this->inputWalltime, this->inputCputime); }
    /**
    * @brief Dumps to the standard output the update times.
    */
	inline void dumpUpdate() { this->dump(this->updateWalltime, this->updateCputime); }
    /**
    * @brief Dumps to the standard output the render times.
    */
	inline void dumpRender() { this->dump(this->renderWalltime, this->renderCputime); }

    /**
    * @brief Retrieves the input wall and cpu times in a Pair<double>.
    *
    * @return a pair containing wall and cpu times
    */
	inline Pair<double> getInputTime() { return {this->inputWalltime, this->inputCputime}; }
    /**
    * @brief Retrieves the update wall and cpu times in a Pair<double>.
    *
    * @return a pair containing wall and cpu times
    */
	inline Pair<double> getUpdateTime() { return {this->updateWalltime, this->updateCputime}; }
    /**
    * @brief Retrieves the render wall and cpu times in a Pair<double>.
    *
    * @return a pair containing wall and cpu times
    */
	inline Pair<double> getRenderTime() { return {this->renderWalltime, this->renderCputime}; }

private:
	LinuxProfiler() = default;

	/// static shared pointer for Singleton
	inline static Shared<LinuxProfiler> s_pointer = nullptr;

    /// input wall time
	double inputWalltime{};
	/// input cpu time
	double inputCputime{};
	/// update wall time
	double updateWalltime{};
	/// update cpu time
	double updateCputime{};
	/// render wall time
	double renderWalltime{};
	/// render cpu time
	double renderCputime{};

	/// start wall time
	std::timespec m_wallBegin{};
	/// end wall time
	std::timespec m_wallEnd{};
	/// start cpu time
	std::timespec m_cpuBegin{};
	/// end cpu time
	std::timespec m_cpuEnd{};
};
#endif
