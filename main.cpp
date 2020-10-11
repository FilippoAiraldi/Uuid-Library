#include <iostream>
#include <uuids.h>

int main()
{
    uuids::uuid id = uuids::uuid::create();
    std::cout << id << std::endl;

    return 0;
}
