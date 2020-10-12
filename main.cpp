#include <iostream>
#include <chrono>

#include <uuids.h>

int main()
{
    for (int i = 0; i < 5; ++i)
    {
        uuids::uuid u = uuids::uuid::create();
        std::cout << u << std::endl;
    }
    std::cout << "Finished!" << std::endl;
    return 0;
}
