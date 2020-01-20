#ifndef __GA__
#define __GA__

#include "Specimen.h"

#include "Mutation.h"
#include "Crossover.h"
#include "Selection.h"

#include "Mutations.h"
#include "Crossovers.h"
#include "Selections.h"

#include "Environment.h"

#include "Exception.h"

namespace GA {

	template <typename SpecimenType>
	struct SpecimenComp
	{
		bool operator ()(const SpecimenType& a, const SpecimenType& b)
		{
			return a.getFitness() < b.getFitness();
		}
	};

}

#endif // !__GA__
