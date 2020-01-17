#ifndef __MEMBER__
#define __MEMBER__

#include <vector>

/** \class Specimen
 * Class template, that simulates single specimen.
 */
template <typename DnaType, typename ChromosomeType,
        typename DnaContainer = std::vector<DnaType>, typename FenotypeContainer = std::vector<ChromosomeType>>
class Specimen
{
    template <typename S>
    friend class SpecimenTraits;

protected:
    using Gene      = DnaType;
    using GeneContainer = DnaContainer;
    using Chromosome    = ChromosomeType;
    using ChromosomeContainer      = FenotypeContainer;

    GeneContainer dna_;
    double fitness_;
public:
    Specimen() : fitness_(0.0) { }
    virtual ~Specimen() { }
    virtual GeneContainer& getDNA() { return dna_; }
    virtual ChromosomeContainer getFenotype () const { //TODO maybe R-value
        return dna_; //TODO do it
    }

    inline int  getFitness() const      { return fitness_; }
    inline void setFitness(int fitness) { fitness_ = fitness; }
};

template <typename SpecimenType>
struct SpecimenTraits
{
    using Gene      = typename SpecimenType::Gene;
    using GeneContainer = typename SpecimenType::GeneContainer;
};

#endif // __MEMBER__
