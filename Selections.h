/*
 *  Set of predefined selection strategies for genetic algorithms
 *
 *  Authors: Michal Swiatek, Michal Sulek
 *	Update:	 19.01.2020
 *
 *	Github repository: https://github.com/MichalKonradSulek/ZPR
 */

#ifndef __SELECTIONS__
#define __SELECTIONS__

#include <vector>
#include <cstdlib>
#include <limits>
#include <algorithm>

#include "Selection.h"

namespace GA {

	/**
	 *	@brief	Selection strategy that picks parents from a certain
	 *			percent of best individuals
	 *
	 *	@details Populations gets sorted and parents are picked from
	 *			 best precentage of individuals
	 */
	template <typename SpecimenType>
	class BestFitnessPercentageSelection : public Selection<SpecimenType>
	{
	public:
		using Population = typename Selection<SpecimenType>::Population;

	private:
		int best_of_percent_;

	public:
		explicit BestFitnessPercentageSelection(int best_of_percent = 10) : best_of_percent_(best_of_percent) { }
		~BestFitnessPercentageSelection() = default;

		Population select(const Population& population, size_t mating_pool_size) override
		{
		    if(population.empty()) return Population();
			Population mating_pool;
			mating_pool.reserve(mating_pool_size);

			Population pop = population;

			std::sort(pop.begin(), pop.end(), [](const auto& a, const auto& b) { return a.getFitness() > b.getFitness(); });

			for (size_t i = 0; i < mating_pool_size; ++i)
			{
				int choice = rand() % int((best_of_percent_ / 100.f) * population.size());
				mating_pool.emplace_back(pop[choice]);
			}

			return mating_pool;
		}
	};

	/**
	 *	@brief	Selection strategy that implements roulette wheel selection
	 *
	 *	@details Each individual has propability to be chosen as parent 
	 *			 proportional to his fitness score. For each wheel spin
	 *			 a parent is selected. This strategy implies fitness
	 *			 preasure over population and can lead to very quick
	 *			 convergence of solutions
	 *
	 *	@note	This strategy is doesn't work with negative fitnesses
	 */
	template <typename SpecimenType>
	class RouletteWheelSelection : public Selection<SpecimenType>
	{
	public:
		using Population = typename Selection<SpecimenType>::Population;

		Population select(const Population& population, size_t mating_pool_size) override
		{
			//	Setup
			Population mating_pool;
			mating_pool.reserve(mating_pool_size);

			generateWheel(population);

			for (size_t i = 0; i < mating_pool_size; ++i)
			{
				double random_fitness = getDouble() * cumultative_fitness.back();

				//	Find last occurence that is < random_fitness
				int index = closest(cumultative_fitness, random_fitness);
				mating_pool.emplace_back(population[index]);
			}

			return mating_pool;
		}

	protected:
		std::vector<double> cumultative_fitness;

		void generateWheel(const Population& population)
		{
			cumultative_fitness.clear();
			cumultative_fitness.reserve(population.size());

			//	Calculate cumulative distribution
			cumultative_fitness.push_back(getFitness(population.front()));
			for (size_t i = 1; i < population.size(); ++i)
				cumultative_fitness.push_back(getFitness(population[i]) + cumultative_fitness.back());
		}

		inline double getFitness(const SpecimenType& specimen)
		{
			double fitness = specimen.getFitness();
			return fitness < 0.0 ? 0.0 : fitness;
		}

		inline double getDouble() const
		{
			return double(rand()) / RAND_MAX;
		}

		inline int closest(const std::vector<double>& vec, double value)
		{
			auto it = std::lower_bound(vec.begin(), vec.end(), value);
			if (it == vec.end())
				return vec.size() - 1;

			return it - vec.begin();
		}
	};

	/**
	 *	@brief	Selection strategy that implements stochastic universal sampling
	 *
	 *	@details Each individual has propability to be chosen as parent
	 *			 proportional to his fitness score but picking parents
	 *			 happens during one wheel spin. This strategy implies
	 *			 fitness preasure over population and can lead to very
	 *			 quick convergence of solutions. This strategy has higher
	 *			 chance to pick more fitting individuals than roulette wheel
	 *
	 *	@note	This strategy is doesn't work with negative fitnesses
	 *	@note	This strategy is a modification of Roulette Wheel
	 *
	 *	@see	GA::RouletteWheelSelection
	 */
	template <typename SpecimenType>
	class StochasticUniversalSamplingSelection : public RouletteWheelSelection<SpecimenType>
	{
	public:
		using Population = typename RouletteWheelSelection<SpecimenType>::Population;
		using Base = RouletteWheelSelection<SpecimenType>;

		Population select(const Population& population, size_t mating_pool_size) override
		{
			//	Setup
			Population mating_pool;
			mating_pool.reserve(mating_pool_size);

			generateWheel(population);

			double step = Base::cumultative_fitness.back() / mating_pool_size;

			//	Pick starting point
			double random_fitness = Base::getDouble() * Base::cumultative_fitness.back();

			for (size_t i = 0; i < mating_pool_size; ++i, random_fitness += step)
			{
				if (random_fitness > Base::cumultative_fitness.back())
					random_fitness = static_cast<int>(random_fitness) % static_cast<int>(Base::cumultative_fitness.back());

				//	Find last occurence that is < random_fitness
				int index = closest(Base::cumultative_fitness, random_fitness);
				mating_pool.emplace_back(population[index]);
			}

			return mating_pool;
		}
	};

	/**
	 *	@brief	Selection strategy that implements rank selection
	 *
	 *	@details Each individual is ranked based on his fitness score.
	 *			 Then a Roulette Wheel is performed based on ranks
	 *
	 *	@note	This strategy accepts negative fitness values
	 *	@note	This strategy is a modification of Roulette Wheel
	 *
	 *	@see	GA::RouletteWheelSelection
	 */
	template <typename SpecimenType>
	class RankSelection : public RouletteWheelSelection<SpecimenType>
	{
	public:
		using Population = typename Selection<SpecimenType>::Population;
		using Base = RouletteWheelSelection<SpecimenType>;

		Population select(const Population& population, size_t mating_pool_size) override
		{
			//	Setup
			Population mating_pool;
			mating_pool.reserve(mating_pool_size);

			Population pop = population;

			std::sort(pop.begin(), pop.end(), [](const auto& a, const auto& b) { return a.getFitness() < b.getFitness(); });

			//	Rank population
			for (int i = 0; i < pop.size(); ++i)
				pop[i].setFitness(i + 1);

			Base::generateWheel(pop);

			for (size_t i = 0; i < mating_pool_size; ++i)
			{
				double random_fitness = Base::getDouble() * Base::cumultative_fitness.back();

				//	Find last occurence that is < random_fitness
				int index = Base::closest(Base::cumultative_fitness, random_fitness);
				mating_pool.emplace_back(pop[index]);
			}

			return mating_pool;
		}
	};

	/**
	 *	@brief Selection strategy implementing tournament selection
	 *
	 *	@details For each parent K randomly chosen individuals are picked.
	 *			 Each gets evaluated and best one becomes selected.
	 *
	 *	@note	This strategy accepts negative fitness values
	 */
	template <typename SpecimenType>
	class TournamentSelection : public Selection<SpecimenType>
	{
	public:
		explicit TournamentSelection(size_t members_per_parent) : members_per_parent_(members_per_parent) { }
		~TournamentSelection() = default;

		using Population = typename RouletteWheelSelection<SpecimenType>::Population;

		Population select(const Population& population, size_t mating_pool_size) override
		{
			//	Setup
			Population mating_pool;
			mating_pool.reserve(mating_pool_size);

			std::vector<int> candidates(members_per_parent_);

			for (size_t i = 0; i < mating_pool_size; ++i)
			{
				pickRandomIndices(candidates, population.size());

				int index = pickParent(candidates, population);
				mating_pool.emplace_back(population[index]);
			}

			return mating_pool;
		}

	protected:
		size_t members_per_parent_;

		inline void pickRandomIndices(std::vector<int>& vec, size_t n)
		{
			for (size_t i = 0; i < members_per_parent_; ++i)
				vec[i] = rand() % n;
		}

		inline int pickParent(const std::vector<int>& vec, const Population& population)
		{
			auto it = std::max_element(vec.begin(), vec.end(), [&population](int a, int b) { return population[a].getFitness() < population[b].getFitness(); });
			return it - vec.begin();
		}
	};

}

#endif // !__SELECTIONS__
