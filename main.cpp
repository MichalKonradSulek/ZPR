#include <iostream>
#include <cstdlib>
#include <ctime>

#include "Test1.h"
#include "Test2.h"

int main()
{
    srand(time(nullptr));

    MyEnvironment  env1(10000);
    MyEnvironment2 env2(10000);

    env1.runSimulation();

    std::cout << "\n\n\n";

    env2.runSimulation();

    return 0;
}
