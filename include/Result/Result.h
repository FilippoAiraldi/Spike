#pragma once

#include <iostream>

// for structured bindings, see:
// https://en.cppreference.com/w/cpp/language/structured_binding

namespace Spike
{
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

    template <typename T = void>
    struct Result
    {
        Result(const T &value, const Duration &duration) : Value(value), Interval(duration) {}
        Result(const Result<T> &) = delete;
        Result(Result<T> &&) = delete;

        virtual ~Result() {}

        const T Value;
        const Duration Interval;
    };

    template <>
    struct Result<void>
    {
        Result(const Duration &duration) : Interval(duration) {}
        Result(const Result<void> &) = delete;
        Result(Result<void> &&) = delete;

        virtual ~Result() {}

        const Duration Interval;
    };

} // namespace Spike
