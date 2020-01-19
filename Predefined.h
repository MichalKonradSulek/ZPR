#ifndef __PREDEFINED__
#define __PREDEFINED__

#include <cstdlib>

#include "Mutation.h"
#include "Crossover.h"
#include "Selection.h"

namespace GA {

	class FlipBitMutation : public MultipleMutation<bool>
	{
	public:
		using Gene = typename Mutation<bool>::Gene;
		using Genotype = typename Mutation<bool>::Genotype;

	public:
		explicit FlipBitMutation(int mutation_chance = MUTATION_CHANCE_PERCENT, int mutation_iterations = 1, int max_mutations = 1) : MultipleMutation<bool>(mutation_chance, mutation_iterations, max_mutations) { }
		~FlipBitMutation() override = default;

		void performMutation(Genotype& genes) const override
		{
			int choice = rand() % genes.size();

			genes[choice] = !genes[choice];
		}
	};

	template <typename GeneType>
	class SwapGeneMutation : public MultipleMutation<GeneType>
	{
	public:
		using Gene = typename Mutation<GeneType>::Gene;
		using Genotype = typename Mutation<GeneType>::Genotype;

	public:
		explicit SwapGeneMutation(int mutation_chance = MUTATION_CHANCE_PERCENT, int mutation_iterations = 1, int max_mutations = 10) : MultipleMutation<GeneType>(mutation_chance, mutation_iterations, max_mutations) { }
		~SwapGeneMutation() = default;

		void performMutation(Genotype& genes) const override
		{
			size_t a = rand() % genes.size();  //TODO tu też generator
			size_t b = rand() % genes.size();

			std::swap(genes[a], genes[b]);
		}
	};

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
