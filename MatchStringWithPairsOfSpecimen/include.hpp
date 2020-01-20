#ifndef __INCLUDE__
#define __INCLUDE__

#include "../ga.hpp"

#include <iostream>
#include <cstdlib>
#include <ctime>

#include <memory>

std::string str = "This is another string that I'm trying to evolve!";

class MySpecimen : public ga::Specimen<char, char>
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

class SpecimenPair : public ga::Specimen<char, char>
{
public:
	using SpecimenPtr = std::shared_ptr<MySpecimen>;

private:
	MySpecimen first;
	MySpecimen second;

	int	generations_;

public:
	explicit SpecimenPair(size_t max_generations = 10) : generations_(rand() % max_generations + 1)
	{
		first	= MySpecimen();
		second	= MySpecimen();
	}

	SpecimenPair(const MySpecimen& mate1, const MySpecimen& mate2, size_t max_generations = 10) : generations_(rand() % max_generations + 1)
	{
		first  = mate1;
		second = mate2;
	}

	Fenotype getFenotype() const
	{
		Fenotype fenotype1 = first.getFenotype();
		Fenotype fenotype2 = second.getFenotype();

		Fenotype fenotype = std::move(fenotype1);

		fenotype.reserve(fenotype.size() + fenotype2.size());

		for (const auto& chromosome : fenotype2)
			fenotype.emplace_back(chromosome);

		return fenotype;
	}

	void print() const
	{
		std::cout << "First: ";
		first.print();

		std::cout << "Second: ";
		second.print();

		std::cout << "Pair fitness: " << getFitness() << ", generations left: " << generations_ << '\n';
	}

	MySpecimen& getFirst()		{ return first; }
	MySpecimen& getSecond()		{ return second; }

	void	generationPassed()		{ --generations_; }
	int 	generationsLeft() const { return generations_; }
};

class CharMutation : public ga::MultipleMutation<char>
{
public:
	CharMutation(int mutation_chance = ga::MUTATION_CHANCE_PERCENT, int max_mutations = 1) : MultipleMutation<char>(mutation_chance, 40, max_mutations) { }
	~CharMutation() = default;

	void performMutation(Genotype& genes) const override
	{
		int choice = rand() % genes.size();

		genes[choice] = rand() % 96 + 32;
	}
};

class MyEnvironment : public ga::Environment<SpecimenPair>
{
private:
	void crossover() override
	{
		/*for (auto& child : mating_pool_)
		{
			if (child.getFirst() && child.getSecond())
				crossover_type_->cross((*child.getFirst()).getGenotype(), (*child.getSecond()).getGenotype());
		}*/

		for (size_t i = 0; i < mating_pool_.size() - 1; i += 2)
		{
			auto first = mating_pool_[i];
			auto second = mating_pool_[i + 1];

			crossover_type_->cross(first.getFirst().getGenotype(), second.getFirst().getGenotype());
			crossover_type_->cross(first.getSecond().getGenotype(), second.getSecond().getGenotype());
		}

		offspring_ = mating_pool_;
	}

	void mutation() override
	{
		for (auto& child : offspring_)
		{
			mutation_type_->mutate(child.getFirst().getGenotype());
			mutation_type_->mutate(child.getSecond().getGenotype());
		}
	}

	void reproduction() override
	{
		std::vector<MySpecimen> individuals;

		for (int i = offspring_.size() - 1; i >= 0; --i)
		{
			if (offspring_[i].generationsLeft() < 0)
			{
				individuals.push_back(std::move(offspring_[i].getFirst()));
				individuals.push_back(std::move(offspring_[i].getSecond()));

				std::swap(offspring_[i], offspring_.back());
				offspring_.pop_back();
			}
			else
				offspring_[i].generationPassed();
		}

		std::random_shuffle(individuals.begin(), individuals.end());

		for (int i = 0; i < (int)individuals.size() - 1; i += 2)
			offspring_.push_back(SpecimenPair(individuals[i], individuals[i + 1]));
		
		population_ = std::move(offspring_);
	}

public:
	explicit MyEnvironment(size_t population_size) : Environment<SpecimenPair>(population_size) { }
};

#endif // !__INCLUDE__

