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

    MyFitness fitness1;
    env1.runSimulation(fitness1);

    std::cout << "\n\n\n";

//    MyFitness2 fitness2;
//    env2.runSimulation(fitness2);

    return 0;
}
