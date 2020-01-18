#ifndef __TEST3__
#define __TEST3__

#include "Specimen.h"
#include "Mutation.h"

#include "Environment.h"

std::string str3 = "Yet another string I'm trying to evolve!";

class MySpecimen3 : public Specimen<char, char>
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

class CharMutation : public MultipleMutation<char>
{
public:
	CharMutation(int mutation_chance = 10, int max_mutations = 1) : MultipleMutation<char>(mutation_chance, max_mutations) { }
	~CharMutation() = default;

	void mutate(Genotype& genes) const override
	{
		int mutations_occured = 0;

		for (auto& gene : genes)
		{
			if (mutationCondition())
				gene = rand() % 96 + 32, ++mutations_occured;

			if (mutations_occured >= max_mutations_)
				return;
		}
	}
};

#endif // !__TEST3__
