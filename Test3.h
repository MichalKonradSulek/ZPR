#ifndef __TEST3__
#define __TEST3__

#include "Specimen.h"
#include "Mutation.h"

#include "Environment.h"

std::string str3 = "Yet another string I'm trying to evolve!";

class MySpecimen3 : public GA::Specimen<char, char>
{
public:
	MySpecimen3()
	{
		dna_.reserve(str3.length());

		for (size_t i = 0; i < str3.length(); ++i)
			dna_.push_back(rand() % 96 + 32);
	}

	Fenotype getFenotype() const override
	{
		return dna_;
	}
};

class CharMutation : public GA::MultipleMutation<char>
{
public:
	CharMutation(int mutation_chance = GA::MUTATION_CHANCE_PERCENT, int max_mutations = 1) : MultipleMutation<char>(mutation_chance, 40, max_mutations) { }
	~CharMutation() = default;

	void mutateOnce(Genotype& genes) const override
	{
		int choice = rand() % genes.size();

		genes[choice] = rand() % 96 + 32;
	}
};

#endif // !__TEST3__
