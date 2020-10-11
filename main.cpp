#include <iostream>
#include <uuids.h>

int main()
{
    auto t1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 5; ++i)
    {
        uuids::uuid u = uuids::uuid::create();
        // std::cout << u << std::endl;
    }
    std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - t1).count() << std::endl;

    std::cout << "Finished!" << std::endl;

    return 0;
}
