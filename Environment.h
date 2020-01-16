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
          typename MutationType = Mutation<typename SpecimenTraits<SpecimenType>::Gene>,
          typename CrossoverType = Crossover<typename SpecimenTraits<SpecimenType>::GeneContainer> >
class Environment
{
    using GeneType = typename SpecimenTraits<SpecimenType>::Gene;
    using GeneContainer = typename SpecimenTraits<SpecimenType>::GeneContainer;

private:
    int populationSize_;
    unsigned long nOfIterations_ = 0;

protected:
    std::vector<SpecimenType> population_;

    std::unique_ptr<Mutation<GeneType>> mutation_;
    std::unique_ptr<Crossover<GeneContainer>>  crossover_;

    virtual double fitness(SpecimenType& member) = 0;
    virtual bool   finishCondition() = 0;

public:
    explicit Environment(int populationSize) : population_(), populationSize_(populationSize)
    {
        mutation_ = std::make_unique<MutationType>();
        crossover_ = std::make_unique<CrossoverType>();
        setPopulation();
    }

    void setMutation(const std::unique_ptr<Mutation<GeneType>> mutation) {
        mutation_.reset(mutation);
    }

    void setCrossover(const std::unique_ptr<Crossover<GeneContainer>> crossover) {
        crossover_.reset(crossover);
    }

    void evaluate() {
        for (auto&& member : population_)
            member.setFitness(fitness(member));
    }

    void selection(std::vector<SpecimenType>& offspring)
    {

        for (size_t i = 0; i < population_.size(); i++)
        {
            int choice = rand() % (population_.size() / 10);
            offspring.at(i) = population_.at(choice);
        }
    }

    void cross(std::vector<SpecimenType>& offspring)
    {
        for (int i = 0; i < offspring.size() - 1; i += 2)
            crossover_->cross(offspring[i].getDNA(), offspring[i + 1].getDNA());
    }

    void mutate(std::vector<SpecimenType>& offspring)
    {
        for (auto& member : offspring)
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

    unsigned long getNOfIterations() const
    {
        return nOfIterations_;
    }

    void iteration() {
        evaluate();
        std::sort(population_.begin(), population_.end(), [](SpecimenType& a, SpecimenType& b){return a.getFitness() > b.getFitness();});
        std::vector<SpecimenType> offspring(population_.size());
        selection(offspring);
        cross(offspring);
        mutate(offspring);
        population_ = std::move(offspring);

        ++nOfIterations_;

        showBest(); //TODO remove this
    }

    void iteration(unsigned long nOfIterations) {
        for(unsigned long i = 0; i < nOfIterations; ++i) {
            iteration();
        }
    }

    void runSimulation()
    {
        while (!finishCondition())
        {
            iteration();
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
