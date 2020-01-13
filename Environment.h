#ifndef __ENVIRONMENT__
#define __ENVIRONMENT__

#include <vector>
#include <iostream>
#include <algorithm>
#include <memory>

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
    int populationSize_;

protected:
    std::vector<SpecimenType> population_;

    std::unique_ptr<Mutation<GeneType>> mutation_;
    CrossoverType   crossover_;

    virtual double fitness(SpecimenType& member) = 0;
    virtual bool   finishCondition() = 0;

public:
    explicit Environment(int populationSize) : population_(), crossover_(), populationSize_(populationSize)
    {
        mutation_ = std::make_unique<MutationType>();
        setPopulation();
    }

    void selection()
    {
        std::vector<SpecimenType> offspring;
        offspring.reserve(population_.size());

        for (size_t i = 0; i < population_.size(); i++)
        {
            int choice = rand() % (population_.size() / 10);
            offspring.emplace_back(population_[choice]);
        }

        for (int i = 0; i < offspring.size() - 1; i += 2)
            crossover_.cross(offspring[i].getDNA(), offspring[i + 1].getDNA());

        population_ = std::move(offspring);
    }

//    void cross()
//    {
//        for (int i = 0; i < population_.size() - 1; i += 2)
//            crossover_.cross(population_[i].getDNA(), population_[i + 1].getDNA());
//    }

    void mutate()
    {
        for (auto& member : population_)
        {
            for (auto&& gene : member.getDNA())
            {
                if (mutation_->mutationCondition())
                    mutation_->mutate(std::forward<GeneType>(gene));
            }
        }
    }

    virtual void setPopulation()
    {
        population_.clear();
        population_.reserve(populationSize_);

        for (int i = 0; i < populationSize_; i++)
            population_.emplace_back();
    }

    void runSimulation()
    {
        while (!finishCondition())
        {
            for (auto&& member : population_)
                member.setFitness(fitness(member));

            std::sort(population_.begin(), population_.end(), [](SpecimenType& a, SpecimenType& b){return a.getFitness() > b.getFitness();});

            selection();
            mutate();

            showBest();
        }
    }

    void showBest()
    {
        std::cout << population_[0].getDNA() << "\tfitness: " << population_[0].getFitness() << '\n'; //TODO dangerous population_[0]
    }

    SpecimenType getBest()
    {
        if(population_.size() == 0) return SpecimenType(); //TODO is this correct?
        return population_[0];
    }
};

#endif // __ENVIRONMENT__
