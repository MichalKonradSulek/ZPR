#include <iostream>
#include <cstdlib>
#include <ctime>

#include "GA.h"

#include "Test1.h"
#include "Test3.h"

int main()
{
    srand(time(nullptr));

	auto finishCondition1 = [](const auto& population)
	{
		GA::SpecimenComp<MySpecimen> comp;

		auto it = std::max_element(population.begin(), population.end(), comp);
		auto fenotype = (*it).getFenotype();

		return str1 == std::string(fenotype.begin(), fenotype.end());
	};

	auto fitness1 = [](const MySpecimen& specimen)
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

	MyEnvironment  env1(5000);

	env1.setMutationType<GA::FlipBitMutation>(GA::MUTATION_CHANCE_PERCENT * 0.05, 100, 5);
	env1.setCrossoverType<GA::SinglePointCrossover>();
	env1.setSelectionType<GA::RankSelection>();

    env1.runSimulation(fitness1, finishCondition1, -1);

	auto fitness3 = [](const MySpecimen3& specimen)
	{
		double result = 0;
		const auto& fenotype = specimen.getFenotype();

		for (size_t i = 0; i < str3.size(); i++)
		{
			if (fenotype[i] == str3[i])
				result += 10;
		}

		return result;
	};

	auto finishCondition3 = [](const auto& population)
	{
		GA::SpecimenComp<MySpecimen3> comp;

		auto it = std::max_element(population.begin(), population.end(), comp);
		auto fenotype = (*it).getFenotype();

		return str3 == std::string(fenotype.begin(), fenotype.end());
	};

	GA::Environment<MySpecimen3>  env3(1000, new CharMutation(GA::MUTATION_CHANCE_PERCENT * 0.01, 10), new GA::UniformCrossover<char>(), new GA::RouletteWheelSelection<MySpecimen3>());

	env3.runSimulation(fitness3, finishCondition3);

    std::cout << "\n\n\n";

    return 0;
}
