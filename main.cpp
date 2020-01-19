#include <iostream>
#include <cstdlib>
#include <ctime>

#include "Test1.h"
#include "Test3.h"

#include "Selections.h"
#include "Mutations.h"
#include "Crossovers.h"

int main()
{
    srand(time(nullptr));

    MyEnvironment  env1(10000);
	GA::Environment<MySpecimen3>  env3(10000);

	auto finishCondition1 = [](const auto& population, auto fitness)
	{
		auto it = std::max_element(population.begin(), population.end(), [](const auto& a, const auto& b) {return a.getFitness() < b.getFitness(); });
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

	//.setMutationType<GA::SwapGeneMutation<bool> >(7, GA::MUTATION_CHANCE_PERCENT * 0.05, str1.length());
	//env1.setMutationType<GA::ScrambleGenesMutation<bool> >(7, GA::MUTATION_CHANCE_PERCENT * 0.05);
	env1.setMutationType<GA::InverseGenesMutation<bool> >(7, GA::MUTATION_CHANCE_PERCENT * 0.05);
	env1.setSelectionType<GA::RankSelection<MySpecimen> >();

    //env1.runSimulation(fitness1, finishCondition1);

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

	auto finishCondition3 = [](const auto& population, auto fitness)
	{
		auto it = std::max_element(population.begin(), population.end(), [](const auto& a, const auto& b) {return a.getFitness() < b.getFitness(); });
		auto fenotype = (*it).getFenotype();
		return str3 == std::string(fenotype.begin(), fenotype.end());
	};

	env3.setMutationType<CharMutation>(GA::MUTATION_CHANCE_PERCENT * 0.5, 10);
	//env3.setMutationType<GA::SwapGeneMutation<char> >(5, GA::MUTATION_CHANCE_PERCENT);
	
	env3.setSelectionType<GA::RankSelection<MySpecimen3> >();

	env3.setCrossoverType<GA::UniformCrossover<char> >();
	//env3.setCrossoverType<GA::MultiplePointCrossover<char> >(5);
	//env3.setCrossoverType<GA::SinglePointCrossover<char> >();

	env3.runSimulation(fitness3, finishCondition3);

	//env3.showBest();

    std::cout << "\n\n\n";

    return 0;
}
