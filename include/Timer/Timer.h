#pragma once

// #include "../Result/Result.h"
#include "Result.h"
#include "../StopWatch/StopWatch.h"
#include <functional>
#include <vector>
#include <algorithm>

#define DEFAULT_NTRAILS 100000

namespace Spike
{
    namespace Timer
    {
        template <typename T>
        Result<T> Measure(const std::function<T()> &func, unsigned int nTrials = DEFAULT_NTRAILS)
        {
            // using constexpr instead of template specialization
            constexpr bool isVoid = std::is_void<T>::value;
            if constexpr (isVoid)
            {
                StopWatch sw = StopWatch::StartNew();
                for (unsigned int i = 0; i < nTrials; ++i)
                    func();
                sw.Stop();
                return Result<void>(sw.Elapsed());
            }
            else
            {
                T res{};
                StopWatch sw = StopWatch::StartNew();
                for (unsigned int i = 0; i < nTrials; ++i)
                    res = func();
                sw.Stop();
                return Result<T>(res, sw.Elapsed());
            }
        }

        // template <>
        // Result<void> Measure(std::function<void()> func, unsigned int nTrials)
        // {
        //     StopWatch sw = StopWatch::StartNew();
        //     for (unsigned int i = 0; i < nTrials; ++i)
        //         func();
        //     sw.Stop();
        //     return Result<void>(sw.Elapsed());
        // }

        template <typename T>
        std::vector<Result<T>> Measure(const std::vector<std::function<T()>> &funcs, unsigned int nTrials = DEFAULT_NTRAILS, bool reversed = false)
        {
            std::vector<Result<T>> results;
            results.reserve(funcs.size());

            auto evaluateEach = [&nTrials](std::function<T()> f) -> Result<T> { return Measure<T>(f, nTrials); };
            if (!reversed)
                std::transform(funcs.cbegin(), funcs.cend(), std::back_inserter(results), evaluateEach);
            else
                std::transform(funcs.crbegin(), funcs.crend(), std::back_inserter(results), evaluateEach);

            return results;
        }

        template <typename T, typename Iterator>
        std::vector<Result<T>> Measure(Iterator begin, Iterator end, unsigned int nTrials = DEFAULT_NTRAILS)
        {
            std::vector<Result<T>> results;
            results.reserve(std::distance(begin, end));

            auto evaluateEach = [&nTrials](std::function<T()> f) -> Result<T> { return Measure<T>(f, nTrials); };
            std::transform(begin, end, std::back_inserter(results), evaluateEach);

            return results;
        }

        template <typename T>
        bool CompareResults(const std::vector<T> &results, const std::function<bool(T, T)> &compare)
        {
            int x;
            x = 5;
            return false;
        }

        /* public
        static bool CompareResults<TResult>(
            IEnumerable<TResult> results,
            Func<TResult, TResult, bool> comp)
        {
            results = results.ToArray();
            var firstRes = results.First();
            return results.All(r = > comp(r, firstRes));
        } */

    }; // namespace Timer

} // namespace Spike
