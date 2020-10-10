#include <iostream>
#include <uuids.h>
#include <fstream>

int main()
{
    // in the paaper, there are 3 algos: time, name, random
    uuids::uuid id1 = uuids::uuid::create(uuids::algorithms::types::time_based);
    // uuids::uuid id2 = uuids::uuid::create(uuids::algorithms::types::name_based);

    std::cout << id1.to_string() << std::endl;

    return 0;
}
