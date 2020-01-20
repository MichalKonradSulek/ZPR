#include "../GA.h"

#include <iostream>
#include <cstdlib>
#include <ctime>

std::string str = "This is another string that I'm trying to evolve!";

class MySpecimen : public GA::Specimen<char, char>
{
public:
	MySpecimen()
	{
		dna_.reserve(str.length());

		for (size_t i = 0; i < str.length(); ++i)
			dna_.push_back(rand() % 96 + 32);
	}

	Fenotype getFenotype() const override
	{
		return dna_;
	}

	void print() const
	{
		auto fenotype = getFenotype();
		std::cout << std::string(fenotype.begin(), fenotype.end()) << "\tfitness: " << getFitness() << '\n';
	}
};

class CharMutation : public GA::MultipleMutation<char>
{
public:
	CharMutation(int mutation_chance = GA::MUTATION_CHANCE_PERCENT, int max_mutations = 1) : MultipleMutation<char>(mutation_chance, 40, max_mutations) { }
	~CharMutation() = default;

	void performMutation(Genotype& genes) const override
	{
		int choice = rand() % genes.size();

		genes[choice] = rand() % 96 + 32;
	}
};

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