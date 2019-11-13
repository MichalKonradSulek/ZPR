#ifndef __MUTATION__
#define __MUTATION__

#include <cstdlib>
#include <iostream>

template <typename GeneType>
class Mutation
{
private:
    int mutationRate;

public:
    Mutation() : mutationRate(10) { }

    virtual bool mutationCondition()
    {
        int chance = rand() % 1000;
        return chance < mutationRate;
    }

    virtual void mutate(GeneType&& gene)
    {
    }

    inline void setMutationRate(int mutationRate) { this->mutationRate = mutationRate; }
};

template <>
void Mutation<char>::mutate(char&& gene)
{
    gene = rand() % 96 + 32;
}

#endif // __MUTATION__
