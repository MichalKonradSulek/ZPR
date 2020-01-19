#ifndef __PREDEFINED__
#define __PREDEFINED__

#include <cstdlib>

#include "Mutation.h"
#include "Crossover.h"
#include "Selection.h"

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

}

#endif // !__PREDEFINED__
