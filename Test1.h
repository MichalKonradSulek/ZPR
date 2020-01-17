#ifndef __TEST1__
#define __TEST1__

#include "Member.h"
#include "Mutation.h"
#include "Environment.h"

std::string str1 = "This is the string I'm trying to evolve!";

const size_t GENES_PER_CHROMOSOME = 7;

class MySpecimen : public Specimen<bool, char, std::string, std::string>
{
public:
    MySpecimen()
    {
        dna_.resize(str1.size() * GENES_PER_CHROMOSOME);
        for (auto& c : dna_)
            c = rand() % 2;
    }
    ChromosomeContainer getFenotype() const override {
        ChromosomeContainer result;
        result.reserve(dna_.size());
        size_t counter = 0;
        char chromosome = 0;
        for(const auto& i : dna_) {
            chromosome *= 2;
            i ? ++chromosome : chromosome;
            ++counter;
            if(counter >= 7) {
                counter = 0;
                result.push_back(chromosome);
                chromosome = 0;
            }
        }
        return result;
    }
};

class MyFitness : public Fitness<MySpecimen>
{
public:
    double rateSpecimen (const Subject& specimen, const std::vector<Subject>& population) const override
    {
        double result = 0;
        const auto& DNA = specimen.getFenotype();
        for (int i = 0; i < str1.size(); i++)
        {
            if (DNA[i] == str1[i])
                result += 10;
            else if (DNA[i] < 32 || DNA[i] > 126)
                result -= 100;
        }
        return result;
    }
};

class MyMutation : public Mutation<bool>
{
    int mutationRate = 10;

public:
    inline bool mutationCondition() final
    {
        int chance = rand() % 1000;
        return chance < mutationRate;
    }

//    void mutate(char&& gene)
//    {
//        gene += rand() % 10 - 5; //TODO gene may reach out of range
//    }
};

class MyEnvironment : public Environment<MySpecimen, MyMutation>
{
private:
    inline bool finishCondition() final
    {
        return population_[0].getFenotype() == str1;
    }

public:
    MyEnvironment(int populationSize) : Environment(populationSize) { }
};

#endif // __TEST1__
