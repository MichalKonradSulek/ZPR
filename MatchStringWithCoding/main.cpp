#include "include.hpp"

int main() {
	srand(time(nullptr));

	auto finishCondition = [](const auto& population)
	{
		ga::SpecimenComp<MySpecimen> comp;

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

	ga::Environment<MySpecimen>  env(5000);

	env.setMutationType<ga::FlipBitMutation>(ga::MUTATION_CHANCE_PERCENT * 0.05, 100, 5);
	env.setCrossoverType<ga::SinglePointCrossover>();
	env.setSelectionType<ga::RankSelection>();

	env.runSimulation(fitness, finishCondition, -1, false);

	auto fenotype = env.getBest().getFenotype();

	std::cout << std::string(fenotype.begin(), fenotype.end()) << '\n';

	return 0;
}