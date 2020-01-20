#ifndef __INCLUDE__
#define __INCLUDE__

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

#endif // !__INCLUDE__

