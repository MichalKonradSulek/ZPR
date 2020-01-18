#ifndef __TEST1__
#define __TEST1__

#include "Specimen.h"
#include "Mutation.h"
#include "Environment.h"

std::string str1 = "This is the string I'm trying to evolve!";

const size_t GENES_PER_CHROMOSOME = 7;

class MySpecimen : public GA::Specimen<bool, char>
{
	//using ChromosomeContainer = Specimen<bool, char>::Ch

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

        for(const auto& gene : dna_)
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
};

class MyEnvironment : public GA::Environment<MySpecimen>
{
private:
    inline bool finishCondition() final
    {
		auto fenotype = population_[0].getFenotype();
		return str1 == std::string(fenotype.begin(), fenotype.end());
    }

public:
    MyEnvironment(int populationSize) : Environment(populationSize) { }
};

#endif // __TEST1__
