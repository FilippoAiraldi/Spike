#include <iostream>
#include <cmath>
#include <Timer/Timer.h>

#include <list>

int MyMethod(int n)
{
    for (int i = 0; i < n; ++i)
    {
        int x = -5;
        int y = abs(x);
        int z = sqrt(y);
    }
    return 42;
}

int main()
{
    auto shortMethod = []() { return MyMethod(1000); };
    auto longMethod = []() { return MyMethod(1000000); };
    std::vector<std::function<int(void)>> funcs = {shortMethod, longMethod};
    auto res = Spike::Timer::Measure<int>(funcs, 3, true);
    for (auto r : res)
    {
        std::cout << r.Interval.ToMicroseconds() << std::endl;
    }

    // std::function<bool(Spike::Result<int>, Spike::Result<int>)> comp = [](Spike::Result<int> &r1, Spike::Result<int> &r2) { return true; };
    std::function<bool(Spike::Result<int>, Spike::Result<int>)> cmp = [](Spike::Result<int> x, Spike::Result<int> y) { return true; };
    bool eq = Spike::Timer::CompareResults(res, cmp);

    /* std::list<std::function<int(void)>> funcs2;
    funcs2.push_back(shortMethod);
    funcs2.push_back(longMethod);
    auto res2 = Spike::Timer::Measure<int>(funcs2.crbegin(), funcs2.crend(), 3);
    for (auto r : res2)
    {
        std::cout << r.Interval.ToMicroseconds() << std::endl;
    } */

    // std::cout << res.Value << std::endl;
    // std::cout << res.Interval.ToMilliseconds() << std::endl;

    // MyMethodToTest();

    // Spike::Duration d(1254);
    // Spike::Result<double> r(5.1, d);

    return 0;
}
