/*
 *  Set of predefined mutation strategies for genetic algorithms
 *
 *  Authors: Michal Swiatek, Michal Sulek
 *	Update:	 19.01.2020
 *
 *	Github repository: https://github.com/MichalKonradSulek/ZPR
 */

#ifndef __MUTATIONS__
#define __MUTATIONS__

#include "mutation.hpp"

#include <cstdlib>
#include <algorithm>

namespace GA {

	/**
	 *	@brief	Mutation strategy aimed specifically for boolean Gene type
	 *
	 *	@details N bits are randomly picked and for each of them there is
	 *			 chance to be flipped equivalent to mutation_chance
	 */
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

	/**
	 *	@brief	Mutation strategy that swaps pairs of Genes
	 *
	 *	@details N pairs of genes are picked and for each of them there is
	 *			 a swap chance equivalent to mutation_chance
	 */
	template <typename GeneType>
	class SwapGeneMutation : public MultipleMutation<GeneType>
	{
	public:
		using Gene = typename Mutation<GeneType>::Gene;
		using Genotype = typename Mutation<GeneType>::Genotype;

		explicit SwapGeneMutation(int swap_range = -1, int mutation_chance = MUTATION_CHANCE_PERCENT, int mutation_iterations = 1, int max_mutations = 10) : MultipleMutation<GeneType>(mutation_chance, mutation_iterations, max_mutations), swap_range_(swap_range) { }
		~SwapGeneMutation() = default;

		void performMutation(Genotype& genes) const override
		{
			size_t a = rand() % genes.size();
			size_t b = rand() % genes.size();
			if (swap_range_ != -1)
				//	Wrap around if index out of bound
				b = (a + (rand() % (2 * swap_range_) - swap_range_)) % genes.size();

			std::swap(genes[a], genes[b]);
		}

	protected:
		int swap_range_;
	};


	/**
	 *	@brief	Mutation strategy that randomly shuffles a range of Genes
	 *
	 *	@details A point and a range of mutation are randomly picked, then
	 *			 genes in range <point, scramble_range) are shuffled
	 */
	template <typename GeneType>
	class ScrambleGenesMutation : public Mutation<GeneType>
	{
	public:
		using Gene = typename Mutation<GeneType>::Gene;
		using Genotype = typename Mutation<GeneType>::Genotype;

		explicit ScrambleGenesMutation(int scramble_range = -1, int mutation_chance = MUTATION_CHANCE_PERCENT) : Mutation<GeneType>(mutation_chance), scramble_range_(scramble_range) { }
		~ScrambleGenesMutation() = default;

		void performMutation(Genotype& genes) const override
		{
			size_t a = rand() % genes.size();
			size_t range = scramble_range_ == -1 ? rand() % genes.size() : rand() % scramble_range_ + 1;
			
			if (a + range >= genes.size())
				std::random_shuffle(genes.begin() + a, genes.end());
			else
				std::random_shuffle(genes.begin() + a, genes.begin() + a + range);
		}

	protected:
		int scramble_range_;
	};

	/**
	 *	@brief	Mutation strategy that inverses a range of Genes
	 *
	 *	@details A point and a range of mutation are randomly picked, then
	 *			 genes in range <point, scramble_range) are inverted
	 */
	template <typename GeneType>
	class InverseGenesMutation : public Mutation<GeneType>
	{
	public:
		using Gene = typename Mutation<GeneType>::Gene;
		using Genotype = typename Mutation<GeneType>::Genotype;

		explicit InverseGenesMutation(int inverse_range, int mutation_chance = MUTATION_CHANCE_PERCENT) : Mutation<GeneType>(mutation_chance), inverse_range_(inverse_range) { }
		~InverseGenesMutation() = default;

		void performMutation(Genotype& genes) const override
		{
			size_t a = rand() % genes.size();
			size_t range = inverse_range_ == -1 ? rand() % genes.size() : rand() % inverse_range_ + 1;

			if (a + range >= genes.size())
				std::reverse(genes.begin() + a, genes.end());
			else
				std::reverse(genes.begin() + a, genes.begin() + a + range);
		}

	protected:
		int inverse_range_;
	};

}

#endif // !__MUTATIONS__
