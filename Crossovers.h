#ifndef __CROSSOVERS__
#define __CROSSOVERS__

#include <vector>
#include <cstdlib>
#include <algorithm>

#include "Crossover.h"

namespace GA {

	template <typename GeneType>
	class SinglePointCrossover : public Crossover<GeneType>
	{
	public:
		using Gene = typename Crossover<GeneType>::Gene;
		using Genotype = typename Crossover<GeneType>::Genotype;

		void cross(Genotype& parentA, Genotype& parentB) override
		{
			size_t crossover_point = rand() % parentA.size(); //TODO generator
			std::swap_ranges(parentA.begin() + crossover_point, parentA.end(), parentB.begin() + crossover_point);
		}
	};

	template <typename GeneType>
	class MultiplePointCrossover : public Crossover<GeneType>
	{
	public:
		using Gene = typename Crossover<GeneType>::Gene;
		using Genotype = typename Crossover<GeneType>::Genotype;

		MultiplePointCrossover() = delete;
		explicit MultiplePointCrossover(int number_of_points) : number_of_points_(number_of_points) { }
		~MultiplePointCrossover() = default;

		void cross(Genotype& parentA, Genotype& parentB) override
		{
			std::vector<int> crossover_points;
			crossover_points.reserve(number_of_points_);

			for (int i = 0; i < number_of_points_; ++i)
				crossover_points.emplace_back(rand() % parentA.size());

			std::sort(crossover_points.begin(), crossover_points.end());

			for (int i = 0; i < number_of_points_ - 1; i += 2)
				std::swap_ranges(parentA.begin() + crossover_points[i], parentA.begin() + crossover_points[i + 1], 
								 parentB.begin() + crossover_points[i]);
		}

	protected:
		int number_of_points_;
	};

	template <typename GeneType>
	class UniformCrossover : public Crossover<GeneType>
	{
	public:
		using Gene = typename Crossover<GeneType>::Gene;
		using Genotype = typename Crossover<GeneType>::Genotype;

		void cross(Genotype& parentA, Genotype& parentB) override
		{
			for (size_t i = 0; i < parentA.size(); ++i)
			{
				if (rand() % 2 == 0)
					std::swap(parentA[i], parentB[i]);
			}
		}
	};

}

#endif // !__CROSSOVERS__
