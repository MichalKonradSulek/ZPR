#ifndef __ENVIRONMENT__
#define __ENVIRONMENT__

#include <vector>
#include <iostream>
#include <algorithm>

#include "Member.h"
#include "Mutation.h"
#include "Crossover.h"

template <typename SpecimenType,
          typename MutationType = Mutation<typename SpecimenTraits<SpecimenType>::GeneType>,
          typename CrossoverType = Crossover<typename SpecimenTraits<SpecimenType>::GeneContainer> >
class Environment
{
    using GeneType = typename SpecimenTraits<SpecimenType>::GeneType;

private:
    int populationSize;

protected:
    std::vector<SpecimenType> population;

    MutationType    mutation;
    CrossoverType   crossover;

    virtual double fitness(SpecimenType& member) = 0;
    virtual bool   finishCondition() = 0;

public:
    Environment(int populationSize) : population(), mutation(), crossover(), populationSize(populationSize)
    {
        setPopulation();
    }

    void selection()
    {
        std::vector<SpecimenType> offspring;
        offspring.reserve(population.size());

        for (size_t i = 0; i < population.size(); i++)
        {
            int choice = rand() % (population.size() / 10);
            offspring.emplace_back(population[choice]);
        }

        for (int i = 0; i < offspring.size() - 1; i += 2)
            crossover.cross(offspring[i].getDNA(), offspring[i + 1].getDNA());

        population = std::move(offspring);
    }

//    void cross()
//    {
//        for (int i = 0; i < population.size() - 1; i += 2)
//            crossover.cross(population[i].getDNA(), population[i + 1].getDNA());
//    }

    void mutate()
    {
        for (auto& member : population)
        {
            for (auto&& gene : member.getDNA())
            {
                if (mutation.mutationCondition())
                    mutation.mutate(std::forward<GeneType>(gene));
            }
        }
    }

    virtual void setPopulation()
    {
        population.clear();
        population.reserve(populationSize);

        for (int i = 0; i < populationSize; i++)
            population.emplace_back();
    }

    void runSimulation()
    {
        while (!finishCondition())
        {
            for (auto&& member : population)
                member.setFitness(fitness(member));

            std::sort(population.begin(), population.end(), [](SpecimenType& a, SpecimenType& b){return a.getFitness() > b.getFitness();});

            selection();
            mutate();

            showBest();
        }
    }

    void showBest()
    {
        std::cout << population[0].getDNA() << "\tfitness: " << population[0].getFitness() << '\n'; //TODO dangerous population[0]
    }
};

#endif // __ENVIRONMENT__
