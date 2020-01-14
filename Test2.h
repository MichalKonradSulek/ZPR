#ifndef __TEST2__
#define __TEST2__

#include "Member.h"
#include "Mutation.h"
#include "Environment.h"
#include "Miscellaneous.h"

std::string str2 = "It's an another string that I'm trying to evolve!";

class MySpecimen2 : public Specimen<char, std::string>
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

    void setMate(int mate, int generations)
    {
        this->generations = generations;
        this->mate = mate;
    }

    int getMate() { return mate; }

    inline int  generationsLeft() const { return generations; }
    inline void deacreaseGeneration() { generations--; }
};

class MyEnvironment2 : public Environment<MySpecimen2>
{
private:
    double fitness(MySpecimen2& member)
    {
        int result = 0;
        const auto& DNA = member.getDNA();
        for (int i = 0; i < str2.size(); i++)
        {
            if (DNA[i] == str2[i])
                result += 10;
        }

        const auto& DNA2 = population_[member.getMate()].getDNA();
        for (int i = 0; i < str2.size(); i++)
        {
            if (DNA2[i] == str2[i])
                result += 10;
        }

        member.deacreaseGeneration();

        return static_cast<double>(result);
    }

    inline bool finishCondition() final
    {
        for (int i = 0; i < population_.size() - 1; i += 2)
        {
            if (population_[i].generationsLeft() <= 0)
            {
                population_[i].setMate(i + 1, 7);
                population_[i + 1].setMate(i, 7);
            }
        }

        return population_[0].getDNA() == str2;
    }

    void setPopulation()
    {
        Environment<MySpecimen2>::setPopulation();

        for (int i = 0; i < population_.size() - 1; i += 2)
        {
            population_[i].setMate(i + 1, 5);
            population_[i + 1].setMate(i, 5);
        }
    }

public:
    MyEnvironment2(int populationSize) : Environment(populationSize) { }
};

#endif // __TEST1__

