#ifndef __GA_UTILITY__
#define __GA_UTILITY__

namespace GA {

	template <typename SpecimenType>
	struct SpecimenComp
	{
		bool operator ()(const SpecimenType& a, const SpecimenType& b)
		{
			return a.getFitness() < b.getFitness();
		}
	};

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

