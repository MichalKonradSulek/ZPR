/*
 *  Class defining environment for genetic algorithm. After setting up
 *  environment settings one function performs whole algorithm with
 *  specified loop (generation happens only once at the beginning):
 *      generate -> evaluate -> selection -> genetic_operators -> succession
 *
 *  Authors: Michal Swiatek, Michal Sulek
 *	Update:	 18.01.2020
 *
 *	Github repository: https://github.com/MichalKonradSulek/ZPR
 */

#ifndef __ENVIRONMENT__
#define __ENVIRONMENT__

#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <memory>

#include "Specimen.h"
#include "Mutation.h"
#include "Crossover.h"
#include "Fitness.h"

template <typename SpecimenType,
          typename MutationType = Mutation<SpecimenType::Gene>,
          typename CrossoverType = Crossover<SpecimenType::Gene> >
class Environment
{
public:
	using size_type = size_t;

	using Subject		= SpecimenType;
    using Gene			= typename SpecimenType::Gene;
    using GeneContainer = std::vector<Gene>;

private:
	size_type population_size_;
	size_type number_of_iterations_ = 0;

protected:
    std::vector<SpecimenType> population_; //TODO this shouldn't be vector	//TODO_2 why?

    std::unique_ptr<Mutation<Gene>> mutation_;
    std::unique_ptr<Crossover<Gene>>  crossover_;

    virtual bool finishCondition() = 0;

public:
    explicit Environment(int populationSize) : population_(), population_size_(populationSize)
    {
        mutation_ = std::make_unique<MutationType>();
        crossover_ = std::make_unique<CrossoverType>();
        setPopulation();
    }

    void setMutation(const std::unique_ptr<Mutation<Gene>> mutation) {
        mutation_.reset(mutation);
    }

    void setCrossover(const std::unique_ptr<Crossover<GeneContainer>> crossover) {
        crossover_.reset(crossover);
    }

    void evaluate(Fitness<Subject>& fitness) {
        std::for_each(population_.begin(), population_.end(),
                [&fitness, this](Subject& member){member.setFitness(fitness.rateSpecimen(member, population_));});
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
            auto& genotype = member.getDNA();
            for (size_t i = 0; i < genotype.size(); ++i)
            {
                if (mutation_->mutationCondition()) {
                    bool gene = genotype.at(i);
                    mutation_->mutate(gene); //TODO how to make it better?
                    genotype.at(i) = gene;
                }
            }
        }
    }

    virtual void setPopulation()
    {
        population_.clear();
        population_.reserve(population_size_);

        for (size_type i = 0; i < population_size_; i++)
            population_.emplace_back();
    }

    size_type getNumberOfIterations() const
    {
        return number_of_iterations_;
    }

    void iteration(Fitness<Subject>& fitness) {
        evaluate(fitness);

        std::sort(population_.begin(), population_.end(), [](SpecimenType& a, SpecimenType& b){return a.getFitness() > b.getFitness();});
        
		std::vector<SpecimenType> offspring(population_.size());
        
		selection(offspring);
        
		cross(offspring);
        mutate(offspring);
        
		population_ = std::move(offspring);

        ++number_of_iterations_;

		showBest();		//	TODO: remove
    }

    void iteration(Fitness<Subject>& fitness, size_type number_of_iterations) {
        for(size_t i = 0; i < number_of_iterations; ++i)
            iteration(fitness);
    }

    void runSimulation(Fitness<Subject>& fitness)
    {
        while (!finishCondition())
            iteration(fitness);
    }

    void showBest()
    {
		auto fenotype = population_[0].getFenotype();
        std::cout << std::string(fenotype.begin(), fenotype.end()) << "\tfitness: " << population_[0].getFitness() << '\n'; //TODO dangerous population_[0]
    }

    SpecimenType getBest()
    {
		if (!population_.size())	throw std::exception("Population vector is empty!");
        return population_[0];
    }
};

#endif // __ENVIRONMENT__
