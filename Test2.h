#ifndef __TEST2__
#define __TEST2__

#include "Specimen.h"
#include "Mutation.h"
#include "Environment.h"

std::string str2 = "It's an another string that I'm trying to evolve!";

class MySpecimen2 : public Specimen<char, char>
{
private:
    int mate; //TODO shouldn't mate be an iterator?
    int generations;

public:
    MySpecimen2() : generations(0)
    {
        dna_.resize(str2.size());
        for (auto& c : dna_)
            c = rand() % 96 + 32;
    }

	ChromosomeContainer getFenotype() const { return dna_; }

    void setMate(int mate, int generations)
    {
        this->generations = generations;
        this->mate = mate;
    }

    int getMate() { return mate; }

    inline int  generationsLeft() const { return generations; }
    inline void deacreaseGeneration() { generations--; }
};

class MyFitness2 : public Fitness<MySpecimen2>
{
public:
    double rateSpecimen (const Subject& specimen, const std::vector<Subject>& population) const override
    {
        double result = 0;
        const auto& DNA = specimen.getFenotype();
        for (size_t i = 0; i < str2.size(); i++)
        {
            if (DNA[i] == str2[i])
                result += 10;
        }
        return result;
    }
};

class MyEnvironment2 : public Environment<MySpecimen2>
{
private:
    inline bool finishCondition() final
    {
        for (size_t i = 0; i < population_.size() - 1; i += 2)
        {
            if (population_[i].generationsLeft() <= 0)
            {
                population_[i].setMate(i + 1, 7);
                population_[i + 1].setMate(i, 7);
            }
        }

		auto fenotype = population_[0].getFenotype();
		return str1 == std::string(fenotype.begin(), fenotype.end());
    }

    void setPopulation()
    {
        Environment<MySpecimen2>::setPopulation();

        for (size_t i = 0; i < population_.size() - 1; i += 2)
        {
            population_[i].setMate(i + 1, 5);
            population_[i + 1].setMate(i, 5);
        }
    }

public:
    MyEnvironment2(int populationSize) : Environment(populationSize) { }
};

#endif // __TEST1__

