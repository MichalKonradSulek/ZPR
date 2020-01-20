#include "include.hpp"

int main() {
	srand(time(nullptr));

	//	Fitness for individual
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

	//	Fitness for pair of individuals
	auto fitnessPair = [&fitness](SpecimenPair& specimen)
	{
		double result = 0.0;

		specimen.getFirst().setFitness(fitness(specimen.getFirst()));
		result += specimen.getFirst().getFitness();

		specimen.getSecond().setFitness(fitness(specimen.getSecond()));
		result += specimen.getSecond().getFitness();

		return result;
	};

	auto finishCondition = [](const auto& population)
	{
		ga::SpecimenComp<SpecimenPair> comp;

		auto it = std::max_element(population.begin(), population.end(), comp);

		return (*it).getFitness() == str.length() * 10 * 2;	//	Length of string * fitness score * 2 individuals
	};

	MyEnvironment env(1000);

	env.setMutationType<CharMutation>(ga::MUTATION_CHANCE_PERCENT * 0.01, 10);
	env.setSelectionType<ga::RankSelection>();

	env.runSimulation(fitnessPair, finishCondition, -1, false);

	env.getBest().print();

	return 0;
}