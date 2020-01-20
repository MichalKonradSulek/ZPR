#ifndef __GA_UTILITY__
#define __GA_UTILITY__

namespace ga {

	/**
	 *	\brief	Compare struct for Specimens
	 *
	 *	\details Compares Specimens by their fitness
	 *
	 *	\tparam	SpecimenType	Type of Specimen to compare
	 */
	template <typename SpecimenType>
	struct SpecimenComp
	{
		bool operator ()(const SpecimenType& a, const SpecimenType& b)
		{
			return a.getFitness() < b.getFitness();
		}
	};


	/**
	 *	\brief	Compare struct for Specimens
	 *
	 *	\details Compares Specimens by their fitness
	 *
	 *	\tparam	SpecimenType	Type of Specimen to compare
	 */
	template <typename SpecimenType>
	struct SpecimenCompReverse
	{
		bool operator ()(const SpecimenType& a, const SpecimenType& b)
		{
			return a.getFitness() > b.getFitness();
		}
	};

}

#endif // !__GA_UTILITY__

