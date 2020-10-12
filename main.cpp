#include <iostream>

#include <uuids.h>
#include <set>

int main()
{
    std::set<uuids::uuid> set;

    for (int i = 0; i < 5; ++i)
    {
        uuids::uuid u = uuids::uuid::create();
        std::cout << u << std::endl;
        set.insert(u);
    }

    std::cout << "Finished!" << std::endl;
    return 0;
}
