#include <iostream>
#include <cstdlib>
#include <ctime>

#include "Test1.h"
#include "Test3.h"

int main()
{
    srand(time(nullptr));

    MyEnvironment  env1(10000);
	GA::Environment<MySpecimen3>  env3(10000);

	auto finishCondition1 = [](const auto& population, auto fitness)
	{
		auto fenotype = population[0].getFenotype();
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
		    else if (fenotype[i] < 32 || fenotype[i] > 126)
		        result -= 5;
		}

		return result;
	};

	env1.setMutationType<GA::FlipBitMutation>(GA::MUTATION_CHANCE_PERCENT * 0.5, GENES_PER_CHROMOSOME * str1.length(), str1.length());
	env1.setSelectionType<GA::BestFitnessSelection<MySpecimen> >(5);

    env1.runSimulation(fitness1, finishCondition1);

	auto fitness3 = [](const MySpecimen3& specimen)
	{
		double result = 0;
		const auto& fenotype = specimen.getFenotype();

		for (size_t i = 0; i < str3.size(); i++)
		{
			if (fenotype[i] == str3[i])
				result += 10;
			else
				result -= 5;
		}

		return result;
	};

	auto finishCondition3 = [](const auto& population, auto fitness)
	{
		auto fenotype = population[0].getFenotype();
		return str3 == std::string(fenotype.begin(), fenotype.end());
	};

	env3.setMutationType<CharMutation>(GA::MUTATION_CHANCE_PERCENT * 0.5, 20);

	env3.runSimulation(fitness3, finishCondition3);

    std::cout << "\n\n\n";

    return 0;
}
