#ifndef __CROSSOVER__
#define __CROSSOVER__

#include <iostream>
#include <cstdlib>

template <typename GeneContainer>
struct Crossover
{
    virtual void cross(GeneContainer& a, GeneContainer& b) { };
};

template <>
void Crossover<std::string>::cross(std::string& a, std::string& b)
{
    int crossPoint = rand() % a.size();

    for (int i = crossPoint; i < a.size(); i++)
        std::swap(a[i], b[i]);
}

#endif // __CROSSOVER__
