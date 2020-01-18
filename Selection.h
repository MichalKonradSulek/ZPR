#ifndef __SELECTION__
#define __SELECTION__

#include <vector>

namespace GA {

	template <typename SpecimenType>
	class Selection
	{
	public:
		using size_type = size_t;

		using Member = SpecimenType;
		using Population = std::vector<SpecimenType>;

		virtual Population select(const Population& population, size_type mating_pool_size) = 0;
	};

}

#endif // !__SELECTION__
