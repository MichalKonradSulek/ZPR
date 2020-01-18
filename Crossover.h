/*
 *	Class representing one of genetic operators - crossover
 *	It is one of key features that allow the whole population
 *	to evolve. It is achieved by mixing partents DNA and 
 *	constructing offspring that has characteristics of both 
 *	of them
 *
 *	Authors: Michal Swiatek, Michal Sulek
 *	Update:	 18.01.2020
 *
 *	Github repository: https://github.com/MichalKonradSulek/ZPR
 */

#ifndef __CROSSOVER__
#define __CROSSOVER__

#include <cstdlib>
#include <vector>

template <typename GeneType>
class Crossover
{
public:
	using GeneContainer = std::vector<GeneType>;

	Crossover() = default;
	~Crossover() = default;

    virtual void cross(GeneContainer& a, GeneContainer& b) { };
};

template <>
void Crossover<char>::cross(std::vector<char>& a, std::vector<char>& b)
{
    int crossPoint = rand() % a.size();

    for (size_t i = crossPoint; i < a.size(); i++)
        std::swap(a[i], b[i]);
}

#endif // __CROSSOVER__
