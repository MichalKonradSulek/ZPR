#ifndef __MEMBER__
#define __MEMBER__

#include <vector>

/** \class Specimen
 * Calss template, that simulates single specimen.
 */
template <typename DnaType, typename DnaContainer = std::vector<DnaType>> //TODO what if <int, std::vector<double>>?
class Specimen
{
    template <typename S>
    friend class SpecimenTraits;

    using GeneType      = DnaType;
    using GeneContainer = DnaContainer;

private:
    double fitness_;

protected:
    GeneContainer dna_;

public:
    Specimen() : fitness_(0.0) { }
    virtual ~Specimen() { }

    inline GeneContainer& getDNA() { return dna_; }

    inline int  getFitness() const      { return fitness_; }
    inline void setFitness(int fitness) { this->fitness_ = fitness; }
};

template <typename SpecimenType>
struct SpecimenTraits
{
    using GeneType      = typename SpecimenType::GeneType;
    using GeneContainer = typename SpecimenType::GeneContainer;
};

#endif // __MEMBER__
