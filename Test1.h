#ifndef __TEST1__
#define __TEST1__

#include "Member.h"
#include "Mutation.h"
#include "Environment.h"
#include "Miscellaneous.h"

std::string str1 = "This is the string I'm trying to evolve!";

bool operator== (const std::vector<unsigned char>& vec, const std::string& str) {
    if(vec.size() != str.size()) return false;
    for(int i = 0; i < vec.size(); ++i) {
        if(vec.at(i) != str.at(i)) return false;
    }
    return true;
}

class MySpecimen : public Specimen<bool, unsigned char>
{
protected:
    ChromosomeType chromosomeFromGenes(typename GeneContainer::iterator firstIterator, typename GeneContainer::iterator secondIterator) const override
    {
        ChromosomeType result(0);
        for(auto i = firstIterator; i != secondIterator; ++i) {
            if(i == dna_.end()); //TODO throw an exception
            result *= 2;
            result += *i;
        }
        return result + 32;
    }
public:
    MySpecimen()
    {
        dna_.resize(7*str1.size()); //TODO 7!!!
        int counter = 0;
        for (auto i = dna_.begin(); i != dna_.end(); ++i) {
            *i = rand() % 2;
            if(counter % 7 == 0) {
                counter = 0;
                beginsOfChromosomes_.push_back(i);
            }
            ++counter;
        }
        beginsOfChromosomes_.push_back(dna_.end());
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

    void mutate(bool&& gene)
    {
        gene = !gene; //TODO gene may reach out of range
    }
};

class MyEnvironment : public Environment<MySpecimen, MyMutation>
{
private:
    double fitness(MySpecimen& member)
    {
        int result = 0;
        auto fenotype = member.getChromosomes();
        for (int i = 0; i < str1.size(); i++)
        {
            if (fenotype[i] == str1[i])
                result += 10;
        }

        return static_cast<double>(result);
    }

    inline bool finishCondition() final
    {
        return population_[0].getChromosomes() == str1;
    }

public:
    MyEnvironment(int populationSize) : Environment(populationSize) { }
};

#endif // __TEST1__
