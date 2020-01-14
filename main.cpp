#include <iostream>
#include <cstdlib>
#include <ctime>

#include "Test1.h"
//#include "Test2.h"

int main()
{
    srand(time(nullptr));

    MyEnvironment  env1(10000);
//    MyEnvironment2 env2(10000);

//    auto temp = env1.getBest().getDNA();
//    std::cout << temp << std::endl;
//
//    auto temp1 = env1.getBest().getChromosomes();
//    std::cout << temp1 << std::endl;

//    auto temp2 = env1.getBest().getChromosomes();
//    for(const auto& i: temp2) {
//        std::cout << static_cast<int>(i) << " ";
//    }
//    std::cout << std::endl;


    env1.runSimulation();

    std::cout << "\n\n\n";

//    env2.runSimulation();

    return 0;
}
