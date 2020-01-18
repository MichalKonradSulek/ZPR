#ifndef __PREDEFINED__
#define __PREDEFINED__

#include <cstdlib>

#include "Mutation.h"
#include "Crossover.h"
#include "Selection.h"

namespace GA {

	class FlipBitMutation : public Mutation<bool>
	{
	public:
		using Gene = typename Mutation<bool>::Gene;
		using Genotype = typename Mutation<bool>::Genotype;

	private:
		int max_mutations_;

	public:
		explicit FlipBitMutation(int mutation_chance = 10, int max_mutations = 1) : Mutation<bool>(mutation_chance), max_mutations_(max_mutations) { }
		~FlipBitMutation() override = default;

		void mutate(Genotype& genes) const override
		{
			int mutations_occured = 0;

			for (auto&& gene : genes) //TODO częściej mutują geny o mniejszych indeksach
			{
				if (mutationCondition())
					gene = !gene, ++mutations_occured;

				if (mutations_occured == max_mutations_)
					return;
			}
		}
	};

	template <typename GeneType>
	class SwapGeneMutation : public Mutation<GeneType>
	{
	public:
		using Gene = typename Mutation<GeneType>::Gene;
		using Genotype = typename Mutation<GeneType>::Genotype;

	private:
		int max_mutations_;

	public:
		explicit SwapGeneMutation(int mutation_chance = 10, int max_mutations = 10) : Mutation<GeneType>(mutation_chance), max_mutations_(max_mutations) { }
		~SwapGeneMutation() = default;

		void mutate(Genotype& genes) const override
		{
			for (int i = 0; i < max_mutations_; ++i)
			{
				size_t a = rand() % genes.size();  //TODO tu też generator
				size_t b = rand() % genes.size();

				std::swap(genes[a], genes[b]);
			}
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

}

#endif // !__PREDEFINED__
