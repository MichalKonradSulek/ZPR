#ifndef __TEST1__
#define __TEST1__

#include "Member.h"
#include "Mutation.h"
#include "Environment.h"

std::string str1 = "This is the string I'm trying to evolve!";

class MySpecimen : public Specimen<char, std::string>
{
public:
    MySpecimen()
    {
        dna_.resize(str1.size());
        for (auto& c : dna_)
            c = rand() % 96 + 32;
    }
};

class MyMutation : public Mutation<char>
{
    int mutationRate = 10;

public:
    inline bool mutationCondition() final
    {
        int chance = rand() % 1000;
        return chance < mutationRate;
    }

    void mutate(char&& gene)
    {
        gene += rand() % 10 - 5; //TODO gene may reach out of range
    }
};

class MyEnvironment : public Environment<MySpecimen, MyMutation>
{
private:
    double fitness(MySpecimen& member)
    {
        int result = 0;
        auto& DNA = member.getDNA();
        for (int i = 0; i < str1.size(); i++)
        {
            if (DNA[i] == str1[i])
                result += 10;
        }

        return static_cast<double>(result);
    }

    inline bool finishCondition() final
    {
        return population_[0].getDNA() == str1;
    }

public:
    MyEnvironment(int populationSize) : Environment(populationSize) { }
};

#endif // __TEST1__
