#ifndef __SELECTIONS__
#define __SELECTIONS__

#include <vector>
#include <cstdlib>
#include <limits>
#include <algorithm>

#include <iostream>

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
	private:
		inline double getFitness(const SpecimenType& specimen)
		{
			double fitness = specimen.getFitness();
			return fitness;
		}

		inline double getDouble() const
		{
			//static int MAX_INT_ = std::numeric_limits<int>::max();
			//static int MAX_INT_ = 10000000;
			//return double(rand() % MAX_INT_) / MAX_INT_;
			return double(rand()) / RAND_MAX;
		}

		inline int closest(const std::vector<double>& vec, double value)
		{
			auto it = std::lower_bound(vec.begin(), vec.end(), value);
			if (it == vec.end()) { return vec.size() - 1; }

			return it - vec.begin();
		}

	public:
		using Population = typename Selection<SpecimenType>::Population;

		RouletteWheelSelection() = default;
		~RouletteWheelSelection() = default;

		Population select(const Population& population, size_t mating_pool_size) override
		{
			//	Setup
			Population mating_pool;
			mating_pool.reserve(mating_pool_size);

			std::vector<double> cumultative_fitness;
			cumultative_fitness.reserve(population.size());

			//	Calculate cumulative distribution
			cumultative_fitness.push_back(getFitness(population.front()));
			for (size_t i = 1; i < population.size(); ++i)
				cumultative_fitness.push_back(getFitness(population[i]) + cumultative_fitness[i - 1]);

			//for (auto& temp : cumultative_fitness)
			//	std::cout << temp << ' ';

			for (size_t i = 0; i < mating_pool_size; ++i)
			{
				double rnd = getDouble();
				double random_fitness = rnd * cumultative_fitness.back();

				//std::cout << rnd << ' ' << random_fitness << ' ' << cumultative_fitness.back() << '\n';

				//	Find last occurence that is < random_fitness
				int index = closest(cumultative_fitness, random_fitness);
				mating_pool.emplace_back(population[index]);
			}

			return mating_pool;
		}
	};

}

#endif // !__SELECTIONS__
