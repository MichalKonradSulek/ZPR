/*
 *	Class representing one of genetic operators - mutation
 *	It is used to maintain diversity within population by
 *	randomly changing parts of DNA of a member and therefore
 *	applying possibly huge changes to environmental fitness
 *
 *	Authors: Michal Swiatek, Michal Sulek
 *	Update:	 18.01.2020
 *
 *	Github repository: https://github.com/MichalKonradSulek/ZPR
 */

#ifndef __MUTATION__
#define __MUTATION__

#include <cstdlib>
#include <vector>

template <typename GeneType>
class Mutation
{
public:
	using Gene		= GeneType;
	using Genotype	= std::vector<Gene>;

private:
	int mutation_chance_;

public:
    explicit Mutation(int mutation_chance = 10) : mutation_chance_(mutation_chance) { }
	virtual ~Mutation() = default;

    virtual bool mutationCondition() const
    {
        int chance = rand() % 1000;
        return chance < mutation_chance_;
    }

	virtual void mutate(Genotype& genes) const = 0;

	inline void getMutationChance() const			   { return mutation_chance_ }
    inline void setMutationChance(int mutation_chance) { mutation_chance_ = mutation_chance; }
};

template <typename GeneType>
class MultipleMutation : public Mutation<GeneType>
{
protected:
	int max_mutations_;

public:
	MultipleMutation(int mutation_chance = 10, int max_mutations = 1) : Mutation<GeneType>(mutation_chance), max_mutations_(max_mutations) { }
	~MultipleMutation() = default;
};

#endif // __MUTATION__
