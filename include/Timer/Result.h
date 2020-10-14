#pragma once

#include "../StopWatch/StopWatch.h"

namespace Spike
{
    // A utility storing the results of a time-performance test.
    template <typename T>
    struct Result
    {
        Result(const T &value, const Duration &duration)
            : Value(std::move(value)),
              Interval(std::move(duration)) {}
        virtual ~Result() {}

        const T Value;
        const Duration Interval;
    };

    // A utility storing the results of a time-performance test.
    template <>
    struct Result<void>
    {
        Result(const Duration &duration)
            : Interval(std::move(duration)) {}
        virtual ~Result() {}

        const Duration Interval;
    };

} // namespace Spike
