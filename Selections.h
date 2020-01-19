#ifndef __SELECTIONS__
#define __SELECTIONS__

#include <vector>
#include <cstdlib>
#include <limits>
#include <algorithm>

#include "Selection.h"

namespace GA {

	template <typename SpecimenType>
	class BestFitnessSelection : public Selection<SpecimenType>
	{
	public:
		using Population = typename Selection<SpecimenType>::Population;

	private:
		int best_of_percent_;

	public:
		explicit BestFitnessSelection(int best_of_percent = 10) : best_of_percent_(best_of_percent) { }
		~BestFitnessSelection() = default;

		Population select(const Population& population, size_t mating_pool_size) override
		{
			Population mating_pool;
			mating_pool.reserve(mating_pool_size);

			for (size_t i = 0; i < mating_pool_size; ++i)
			{
				int choice = rand() % int((best_of_percent_ / 100.f) * population.size()); //TODO generator
				mating_pool.emplace_back(population[choice]);
			}

			return mating_pool;
		}
	};

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

	template <typename SpecimenType>
	class StochasticUniversalSamplingSelection : public RouletteWheelSelection<SpecimenType>
	{
	public:
		using Population = typename RouletteWheelSelection<SpecimenType>::Population;

		Population select(const Population& population, size_t mating_pool_size) override
		{
			//	Setup
			Population mating_pool;
			mating_pool.reserve(mating_pool_size);

			generateWheel(population);

			double step = cumultative_fitness.back() / mating_pool_size;

			//	Pick starting point
			double random_fitness = getDouble() * cumultative_fitness.back();

			for (size_t i = 0; i < mating_pool_size; ++i, random_fitness += step)
			{
				if (random_fitness > cumultative_fitness.back())
					random_fitness = static_cast<int>(random_fitness) % static_cast<int>(cumultative_fitness.back());

				//	Find last occurence that is < random_fitness
				int index = closest(cumultative_fitness, random_fitness);
				mating_pool.emplace_back(population[index]);
			}

			return mating_pool;
		}
	};

	template <typename SpecimenType>
	class RankSelection : public RouletteWheelSelection<SpecimenType>
	{
	public:
		using Population = typename Selection<SpecimenType>::Population;

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

			generateWheel(pop);

			for (size_t i = 0; i < mating_pool_size; ++i)
			{
				double random_fitness = getDouble() * cumultative_fitness.back();

				//	Find last occurence that is < random_fitness
				int index = closest(cumultative_fitness, random_fitness);
				mating_pool.emplace_back(pop[index]);
			}

			return mating_pool;
		}
	};

	template <typename SpecimenType>
	class TournamentSelection : public Selection<SpecimenType>
	{
	public:
		TournamentSelection(size_t members_per_parent) : members_per_parent_(members_per_parent) { }
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
