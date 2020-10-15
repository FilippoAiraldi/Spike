#pragma once

#include "Result.h"
#include "../StopWatch/StopWatch.h"
#include <functional>
#include <vector>
#include <algorithm>

namespace Spike
{
    namespace Evaluator
    {
        template <typename T>
        Result<T> MeasureFunction(const std::function<T()> &func, uint64_t nTrials)
        {
            T res{};
            StopWatch sw = StopWatch::StartNew();
            for (uint64_t i = 0; i < nTrials; ++i)
                res = func();
            sw.Stop();
            return Result<T>(res, sw.Elapsed());
        }

        template <>
        Result<void> MeasureFunction(const std::function<void()> &func, uint64_t nTrials)
        {
            StopWatch sw = StopWatch::StartNew();
            for (uint64_t i = 0; i < nTrials; ++i)
                func();
            sw.Stop();
            return Result<void>(sw.Elapsed());
        }

        template <typename T, typename InIterator, typename OutIterator>
        void MeasureFunctions(InIterator begin, InIterator end, OutIterator destination, uint64_t nTrials)
        {
            // remeber to specify the first template (e.g., Measure<int>), otherwise the compiler might fail
            // destination must be similar to: container<Result<T>>::iterator (vector, list, ...)
            auto evaluateEach = [&nTrials](std::function<T()> f) -> Result<T> { return MeasureFunction<T>(f, nTrials); };
            std::transform(begin, end, destination, evaluateEach);
        }

        template <typename T, typename Iterator>
        std::vector<Result<T>> MeasureFunctions(Iterator begin, Iterator end, uint64_t nTrials)
        {
            std::vector<Result<T>> results;
            results.reserve(std::distance(begin, end));
            MeasureFunctions<T>(begin, end, std::back_inserter(results), nTrials);
            return results;
        }

        template <typename T>
        std::vector<Result<T>> MeasureFunctions(const std::vector<std::function<T()>> &funcs, uint64_t nTrials, bool reversed = false)
        {
            return reversed
                       ? MeasureFunctions<T>(funcs.crbegin(), funcs.crend(), nTrials)
                       : MeasureFunctions<T>(funcs.cbegin(), funcs.cend(), nTrials);
        }

        template <typename T, typename Iterator>
        bool CompareResults(Iterator begin, Iterator end, const std::function<bool(T, T)> &compare)
        {
            if (std::distance(begin, end) == 0UL)
                return true;
            T ref = *begin;
            return std::all_of(begin, end, [&ref, &compare](T obj) { return compare(obj, ref); });
        }

        template <typename T>
        bool CompareResults(const std::vector<T> &results, const std::function<bool(T, T)> &compare)
        {
            return CompareResults(results.cbegin(), results.cend(), compare);
        }

        template <typename T, typename Iterator>
        bool CompareResultsFromFunctions(Iterator begin, Iterator end, const std::function<bool(T, T)> &compare)
        {
            if (std::distance(begin, end) == 0UL)
                return true;
            T ref = (*begin)();
            auto compareEach = [&ref, &compare](std::function<T()> f) -> bool { return compare(f(), ref); };
            return std::all_of(begin, end, compareEach);
        }

        template <typename T>
        bool CompareResultsFromFunctions(const std::vector<std::function<T()>> &funcs, const std::function<bool(T, T)> &compare)
        {
            return CompareResultsFromFunctions(funcs.cbegin(), funcs.cend());
        }
    }; // namespace Evaluator

} // namespace Spike
