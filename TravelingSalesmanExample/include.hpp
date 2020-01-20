#ifndef __INCLUDE__
#define __INCLUDE__

#include "../ga.hpp"

#include <iostream>
#include <ctime>
#include <cstdlib>

#include <array>
#include <utility>
#include <algorithm>

//	Max x and y
const int DIMENSIONS = 100;

constexpr int NUMBER_OF_CITIES = 25;

class Specimen : public ga::Specimen<int, int>
{
public:
	Specimen()
	{
		dna_.reserve(NUMBER_OF_CITIES);
		for (int i = 0; i < NUMBER_OF_CITIES; ++i)
			dna_.emplace_back(i);

		std::random_shuffle(dna_.begin(), dna_.end());
	}

	Fenotype getFenotype() const override
	{
		return dna_;
	}

	void print() const override
	{
		for (const auto& city : dna_)
			std::cout << city << ' ';

		ga::Specimen<int, int>::print();
	}
};

class UniqueCrossover : public ga::Crossover<int>
{
public:
	void cross(Genotype& parentA, Genotype& parentB) override
	{
		Genotype newA;	newA.reserve(parentA.size());
		Genotype newB;  newB.reserve(parentB.size());

		size_t cross_point = rand() % parentA.size();

		auto itA = parentA.begin() + cross_point;
		auto itB = parentB.begin() + cross_point;

		for (size_t i = 0; i < parentA.size(); ++i)
		{
			//	Insert genes only if they are unique
			if (std::find(itB, parentB.end(), parentB[i]) == parentB.end())
				newA.emplace_back(parentB[i]);

			if (std::find(itA, parentA.end(), parentA[i]) == parentA.end())
				newB.emplace_back(parentA[i]);
		}

		for (; itA != parentA.end(); ++itA, ++itB)
		{
			newA.emplace_back(*itB);
			newB.emplace_back(*itA);
		}

		parentA = std::move(newA);
		parentB = std::move(newB);
	}
};

#endif // !__INCLUDE__

