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
	using Gene		= GeneType;
	using Genotype  = std::vector<Gene>;

    virtual void cross(Genotype& parent1, Genotype& parent2) = 0;
};

#endif // __CROSSOVER__
