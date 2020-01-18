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

template <typename GeneType>
class Mutation
{
public:
	using Gene = GeneType;

private:
    size_t mutation_rate_;

public:
    explicit Mutation(size_t mutationRate = 10) : mutation_rate_(mutationRate) { }
	virtual ~Mutation() = default;

    virtual bool mutationCondition()
    {
        unsigned chance = rand() % 1000;
        return chance < mutation_rate_;
    }

    virtual void mutate(GeneType& gene)
    {
    }

	inline void getMutationRate() const			   { return mutation_rate_ }
    inline void setMutationRate(int mutation_rate) { mutation_rate_ = mutation_rate; }
};

template <>
void Mutation<char>::mutate(char& gene)
{
    gene = rand() % 96 + 32;
}

template<>
void Mutation<bool>::mutate(bool& gene)
{
    gene = !gene;
}

#endif // __MUTATION__
