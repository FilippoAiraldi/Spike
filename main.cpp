#include <iostream>
#include <SpikeTester.h>

int main()
{
    std::cout << SpikeTester::Timer::GetResult().x << std::endl;
    return 0;
}
