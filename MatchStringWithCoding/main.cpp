#include "../GA.h"

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <algorithm>

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

int main() {
	srand(time(nullptr));

	auto finishCondition = [](const auto& population)
	{
		GA::SpecimenComp<MySpecimen> comp;

		auto it = std::max_element(population.begin(), population.end(), comp);
		auto fenotype = (*it).getFenotype();

		return str1 == std::string(fenotype.begin(), fenotype.end());
	};

	auto fitness = [](const MySpecimen& specimen)
	{
		double result = 0;
		const auto& fenotype = specimen.getFenotype();

		for (size_t i = 0; i < str1.size(); i++)
		{
			if (fenotype[i] == str1[i])
				result += 10;
		}

		return result;
	};

	GA::Environment<MySpecimen>  env(5000);

	env.setMutationType<GA::FlipBitMutation>(GA::MUTATION_CHANCE_PERCENT * 0.05, 100, 5);
	env.setCrossoverType<GA::SinglePointCrossover>();
	env.setSelectionType<GA::RankSelection>();

	env.runSimulation(fitness, finishCondition, -1, false);

	auto fenotype = env.getBest().getFenotype();

	std::cout << std::string(fenotype.begin(), fenotype.end()) << '\n';

	return 0;
}