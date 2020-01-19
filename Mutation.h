/*
 *	Class representing one of genetic operators - mutation
 *	It is used to maintain diversity within population by
 *	randomly changing parts of DNA of a member and therefore
 *	applying possibly huge changes to environmental fitness
 *
 *	Authors: Michal Swiatek, Michal Sulek
 *	Update:	 19.01.2020
 *
 *	Github repository: https://github.com/MichalKonradSulek/ZPR
 */

#ifndef __MUTATION__
#define __MUTATION__

#include <cstdlib>
#include <vector>

#include "Exception.h"

namespace GA {

	const int MAX_MUTATION_CHANCE		= 100000;
	const int MUTATION_CHANCE_PERCENT	= MAX_MUTATION_CHANCE / 100;

	/*
	 *	@brief Base class representing mutation of Genotype
	 *
	 *	@details Mutation of Genotype occurs trough mutate() function
	 *			 call, this function is pure virtual and must be overriden.
	 *			 Frequency of mutation is controled by mutation_chance and 
	 *			 mutationCondition(), mutation is used to maintain genetic
	 *			 diversity, it is applied after crossover on meating pool
	 *
	 *	@note	void mutate(Genotype& genes) must be overriden
	 *
	 *	@tparam GeneType Type of Gene that mutation will affect
	 *
	 *	@see	GA::Specimen
	 *	@see	GA::Crossover
	 *	@see	GA::Selection
	 */
	template <typename GeneType>
	class Mutation
	{
	public:
		using Gene = GeneType;
		using Genotype = std::vector<Gene>;

	private:
		int mutation_chance_;

	protected:
        virtual bool mutationCondition() const
        {
            int chance = rand() % MAX_MUTATION_CHANCE; //TODO that should be from generator
            return chance < mutation_chance_;
        }

	public:
		explicit Mutation(int mutation_chance = MUTATION_CHANCE_PERCENT) : mutation_chance_(mutation_chance) { }
		virtual ~Mutation() = default;

		/*
		 *	@brief Condition deciding whether a mutation should occur.
		 *		   
		 *	@details By default it checks if a randomly picked number is lower
		 *		     than mutation_chance, override this function to change
		 *			 mutation criteria
		 */
		virtual void mutate(Genotype& genes) const = 0;

		inline int getMutationChance() const { return mutation_chance_; }

		/*
		 *	@brief Use MUTATION_CHANCE_PERCENT to define percentage of mutation chance
		 */
		inline void setMutationChance(int mutation_chance) { mutation_chance_ = mutation_chance; }
	};

	/*
	 *	@brief	Helper class allowing mutations to occur more than once on
	 *			specified Genotype
	 *
	 *	@details Override mutateOnce() to change mutation behaviour or
	 *			 mutatate() to change change criteria of multiple mutation
	 *
	 *	@tparam	GeneType Type of Gene that mutation will affect
	 */
	template <typename GeneType>
	class MultipleMutation : public Mutation<GeneType>
	{
	public:
	    using Genotype = typename Mutation<GeneType>::Genotype;
	private:
		/*
		 *	@brief  Equivalent of mutate() in regular Mutation
		 *
		 *	@note	Must be overriden  
		 */
		virtual void mutateOnce(Genotype& genes) const = 0;

	protected:
		int max_mutations_;
		int mutation_iterations_;

	public:
		/*
		 *	@brief	Define conditions for multiple mutation
		 *
		 *	@details Set max_mutations to -1 to allow for unlimited number of mutations
		 *
		 *	@param	mutation_chance		Chance for mutation per mutation iteration
		 *	@param	mutation_iterations	Number of iterations of mutation per mutate() call
		 *	@param	max_mutations		Number of maximum mutations per mutate() call
		 */
		explicit MultipleMutation(int mutation_chance = MUTATION_CHANCE_PERCENT, int mutation_iterations = 1, int max_mutations = -1) : Mutation<GeneType>(mutation_chance), mutation_iterations_(mutation_iterations), max_mutations_(max_mutations) { }
		~MultipleMutation() = default;

		/*
		 *	@brief	Specifies criteria of multiple mutations
		 */
		void mutate(Genotype& genes) const override
		{
			if (max_mutations_ == -1)
			{
				for (int i = 0; i < mutation_iterations_; ++i)
					mutateOnce(genes);
			}
			else
			{
				for (int i = 0, mutations_occured = 0; i < mutation_iterations_ && mutations_occured < max_mutations_; ++i)
				{
					if (Mutation<GeneType>::mutationCondition())
					{
						mutateOnce(genes);
						++mutations_occured;
					}
				}
			}
		}
		
	};

}

#endif // __MUTATION__
