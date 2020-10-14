#pragma once

#include <cstdint>
#include <chrono>

namespace Spike
{
    // A utility struct representing a duration.
    struct Duration
    {
    public:
        Duration(uint64_t nanoseconds) : t(nanoseconds) {}
        virtual ~Duration() {}

        double ToSeconds() const { return t / 1e9; }
        double ToMilliseconds() const { return t / 1e6; }
        double ToMicroseconds() const { return t / 1e3; }
        uint64_t ToNanoseconds() const { return t; }

    private:
        uint64_t t;
    };

    // A utility class able to measure the time elapsed between two targets.
    struct StopWatch
    {
        typedef std::chrono::high_resolution_clock hrc;
        typedef std::chrono::nanoseconds nanos;

    public:
        StopWatch(bool startNow) : started(startNow), stopped(false)
        {
            if (startNow)
                this->startTime = hrc::now();
        }
        virtual ~StopWatch() {}

        void Start()
        {
            this->started = true;
            this->startTime = hrc::now();
        }
        void Reset()
        {
            if (!this->started)
                return;
            this->startTime = hrc::now();
        }
        void Stop()
        {
            this->stopTime = hrc::now();
            this->stopped = true;
        }
        Duration Elapsed() const
        {
            if (!this->started)
                return 0UL;
            if (this->stopped)
                return (uint64_t)std::chrono::duration_cast<nanos>(stopTime - startTime).count();
            return (uint64_t)std::chrono::duration_cast<nanos>(hrc::now() - startTime).count();
        }

        static StopWatch StartNew() { return StopWatch(true); }

    private:
        bool started;
        bool stopped;
        std::chrono::time_point<hrc> startTime;
        std::chrono::time_point<hrc> stopTime;
    };
} // namespace Spike
