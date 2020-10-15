#include <iostream>
#include <cmath>
#include <Evaluator/Evaluator.h>

#include <list>

int MyMethod(int n)
{
    for (int i = 0; i < n; ++i)
    {
        int x = -5;
        int y = abs(x);
        int z = sqrt(y);
    }
    return n;
}

int main()
{
    auto shortMethod = []() { return MyMethod(100); };
    auto longMethod = []() { return MyMethod(1000); };
    std::vector<std::function<int()>> funcs = {shortMethod, longMethod};

    std::function<bool(int, int)> cmp = [](int i, int j) { return i == j; };
    auto res = Spike::Evaluator::CompareResultsFromFunctions(funcs.cbegin(), funcs.cend(), cmp);

    return 0;
}
