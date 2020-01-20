#include "../GA.h"

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <algorithm>

#ifndef __INCLUDE__
#define __INCLUDE__

std::string str1 = "This is the string I'm trying to evolve!";

const size_t GENES_PER_CHROMOSOME = 7;

class MySpecimen : public GA::Specimen<bool, char>
{
public:
	MySpecimen()
	{
		dna_.resize(str1.size() * GENES_PER_CHROMOSOME);
		for (auto&& c : dna_)
			c = rand() % 2;
	}

	Fenotype getFenotype() const override
	{
		Fenotype result;
		result.reserve(dna_.size());

		size_t counter = 0;
		char chromosome = 0;

		for (const auto& gene : dna_)
		{
			chromosome *= 2;
			gene ? ++chromosome : chromosome;
			++counter;

			if (counter >= 7)
			{
				result.push_back(chromosome);

				chromosome = 0;
				counter = 0;
			}
		}

		return result;
	}

	void print() const
	{
		auto fenotype = getFenotype();
		std::cout << std::string(fenotype.begin(), fenotype.end()) << "\tfitness: " << getFitness() << '\n';
	}
};

#endif // !__INCLUDE__

