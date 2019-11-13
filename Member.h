#ifndef __MEMBER__
#define __MEMBER__

#include <vector>

template <typename T, typename Container = std::vector<T> > //TODO what if <int, std::vector<double>>?
class Specimen
{
    template <typename S>
    friend class SpecimenTraits;

    using GeneType      = T;
    using GeneContainer = Container;

private:
    double fitness;

protected:
    GeneContainer DNA;

public:
    Specimen() : fitness(0.0) { }
    virtual ~Specimen() { }

    inline GeneContainer& getDNA() { return DNA; }

    inline int  getFitness() const      { return fitness; }
    inline void setFitness(int fitness) { this->fitness = fitness; }
};

template <typename SpecimenType>
struct SpecimenTraits
{
    using GeneType      = typename SpecimenType::GeneType;
    using GeneContainer = typename SpecimenType::GeneContainer;
};

#endif // __MEMBER__
