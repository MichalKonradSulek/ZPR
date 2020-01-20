/**
 *  Set of predefined crossover strategies for genetic algorithms
 *
 *  Authors: Michal Swiatek, Michal Sulek
 *	Update:	 19.01.2020
 *
 *	Github repository: https://github.com/MichalKonradSulek/ZPR
 */

#ifndef __CROSSOVERS__
#define __CROSSOVERS__

#include <vector>
#include <cstdlib>
#include <algorithm>

#include "crossover.hpp"

namespace ga {

	/**
	 *	@brief	Crossover strategy with only one crossover point
	 *
	 *	@details Crossover point is randomly picked, then all genes
	 *			 after that crossover point get swapped with other parent
	 */
	template <typename GeneType>
	class SinglePointCrossover : public Crossover<GeneType>
	{
	public:
		using Gene = typename Crossover<GeneType>::Gene;
		using Genotype = typename Crossover<GeneType>::Genotype;

		void cross(Genotype& parentA, Genotype& parentB) override
		{
			size_t crossover_point = rand() % parentA.size();
			std::swap_ranges(parentA.begin() + crossover_point, parentA.end(), parentB.begin() + crossover_point);
		}
	};

	/**
	 *	@brief	Crossover strategy with multiple crossover points
	 *
	 *	@details Crossover points are randomly picked, then genes are
	 *			 crossed between each non-overlaping section
	 */
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

	/**
	 *	@brief	Crossover strategy treating each gene pair separately
	 *
	 *	@details For ech gene of parents there is a 50% chance to be crossed
	 */
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

	template <typename GeneType>
	class NoCrossover : public Crossover<GeneType>
	{
	public:
		using Gene = typename Crossover<GeneType>::Gene;
		using Genotype = typename Crossover<GeneType>::Genotype;

		void cross(Genotype& parentA, Genotype& parentB) override
		{
			
		}
	};

}

#endif // !__CROSSOVERS__
