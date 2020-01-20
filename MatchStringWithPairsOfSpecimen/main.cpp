#include "include.hpp"

int main() {
	srand(time(nullptr));

	auto fitness = [](const MySpecimen& specimen)
	{
		double result = 0;
		const auto& fenotype = specimen.getFenotype();

		for (size_t i = 0; i < str.size(); i++)
		{
			if (fenotype[i] == str[i])
				result += 10;
		}

		return result;
	};

	auto finishCondition = [](const auto& population)
	{
		GA::SpecimenComp<MySpecimen> comp;

		auto it = std::max_element(population.begin(), population.end(), comp);
		auto fenotype = (*it).getFenotype();

		return str == std::string(fenotype.begin(), fenotype.end());
	};

	GA::Environment<MySpecimen>  env(1000, new CharMutation(GA::MUTATION_CHANCE_PERCENT * 0.01, 10), new GA::UniformCrossover<char>(), new GA::RouletteWheelSelection<MySpecimen>());

	env.runSimulation(fitness, finishCondition, -1, false);

	auto fenotype = env.getBest().getFenotype();

	std::cout << std::string(fenotype.begin(), fenotype.end()) << '\n';

	return 0;
}