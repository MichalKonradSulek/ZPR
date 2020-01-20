/**
 *	Class representing one of genetic operators - crossover
 *	It is one of key features that allow the whole population
 *	to evolve. It is achieved by mixing partents DNA and 
 *	constructing offspring that has characteristics of both 
 *	of them
 *
 *	Authors: Michal Swiatek, Michal Sulek
 *	Update:	 19.01.2020
 *
 *	Github repository: https://github.com/MichalKonradSulek/ZPR
 */

#ifndef __CROSSOVER__
#define __CROSSOVER__

#include <cstdlib>
#include <vector>

namespace GA {

	/**
	 *	@brief Base class representing crossover of two individuals
	 *
	 *	@details Abstract class defining interface for Crossover
	 *			 Crossover is applied after selection on chosen mating pool
	 *			 It is used to mix genetic information contained within parents,
	 *			 then mutation is applied
	 *
	 *	@param	GeneType Type of Gene in Genotype, used to determine DNA type
	 *
	 *	@note	cross(Genotype& parent1, Genotype& parent2) must be overriden
	 *
	 *	@see	GA::Specimen
	 *	@see	GA::Mutation
	 *	@see	GA::Selection
	 */
	template <typename GeneType>
	class Crossover
	{
	public:
		using Gene = GeneType;
		using Genotype = std::vector<Gene>;

		/**
		 *	@brief	Function defining crossover method
		 *
		 *	@note	This method has to be overriden
		 */
		virtual void cross(Genotype& parent1, Genotype& parent2) = 0;
	};

}

#endif // __CROSSOVER__
