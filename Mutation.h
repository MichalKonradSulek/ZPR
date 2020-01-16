#ifndef __MUTATION__
#define __MUTATION__

#include <cstdlib>
#include <iostream>

template <typename GeneType>
class Mutation
{
private:
    unsigned mutationRate_;
public:
    explicit Mutation(unsigned mutationRate = 10) : mutationRate_(mutationRate) { }

    virtual bool mutationCondition()
    {
        unsigned chance = rand() % 1000;
        return chance < mutationRate_;
    }

    virtual void mutate(GeneType&& gene)
    {
    }

    inline void setMutationRate(int mutationRate) { mutationRate_ = mutationRate; }
};

template <>
void Mutation<char>::mutate(char&& gene)
{
    gene = rand() % 96 + 32;
}

#endif // __MUTATION__
