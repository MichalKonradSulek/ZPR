/*
 *	Class representing selection of the most suitable
 *	individuals from population. There are different strategies
 *	of selection - this is an Interface for all of them
 *
 *	Authors: Michal Swiatek, Michal Sulek
 *	Update:	 19.01.2020
 *
 *	Github repository: https://github.com/MichalKonradSulek/ZPR
 */

#ifndef __SELECTION__
#define __SELECTION__

#include <vector>

namespace ga {

	/**
	 *	@brief	Base class representing selection of most fitting individuals
	 *
	 *	@details Abstract class defining Interface for selecting a mating pool
	 *			 from given population. The mating pool is chosen from population
	 *			 after evaluation - updated fitness score is held within Specimen
	 *
	 *	@tparam	SpecimenType Type of a member of population
	 *
	 *	@note	select(const Population& population, size_type mating_pool_size) must be overriden
	 *
	 *	@see	Specimen
	 */
	template <typename SpecimenType>
	class Selection
	{
	public:
		using size_type = size_t;

		using Member = SpecimenType;
		using Population = std::vector<SpecimenType>;

		/**
		 *	@brief	A strategy for picking individuals for mating pool
		 *
		 *	@details This function takes a population and generates new mating pool
		 *			 applying a specified strategy
		 *	
		 *	@param	population		 Evaluated population from previous generation
		 *	@param	mating_pool_size Size of generated mating_pool
		 *
		 *	@return Generated population - return plain vector<SpecimenType>, compiler
		 *			should apply RVO otherwise move assigment operator of vector will be used
		 *
		 *	@note	This function has to be overriden
		 */
		virtual Population select(const Population& population, size_type mating_pool_size) = 0;
	};

}

#endif // !__SELECTION__
