#ifndef __MEMBER__
#define __MEMBER__

#include <vector>

/** \class Specimen
 * Class template, that simulates single specimen.
 */
template <typename GType, typename CType,
        template <typename T, typename AllocatorG> class GContainer = std::vector,
        template <typename E, typename AllocatorC> class CContainer = std::vector>
class Specimen
{
    template <typename S>
    friend class SpecimenTraits;

protected:

    using GeneType = GType;
    using ChromosomeType = CType;
    using GeneContainer = GContainer<GType, std::allocator<GType>>;
    using ChromosomeContainer = CContainer<CType, std::allocator<CType>>;

    double fitness_;
    GeneContainer dna_;
public: //TODO public!
    std::vector<typename GeneContainer::iterator> beginsOfChromosomes_;
    virtual ChromosomeType chromosomeFromGenes(typename GeneContainer::iterator begin, typename GeneContainer::iterator end) const = 0;
public:
    Specimen() : fitness_(0.0) { }
    virtual ~Specimen() = default;
    virtual GeneContainer& getDNA() { return dna_; }
    virtual int  getFitness() const      { return fitness_; }
    virtual void setFitness(int fitness) { this->fitness_ = fitness; }
    virtual ChromosomeContainer getChromosomes() const
    {
        ChromosomeContainer result;
        result.reserve(beginsOfChromosomes_.size());
        auto firstIterator = beginsOfChromosomes_.begin();
        auto secondIterator = beginsOfChromosomes_.begin();
        if(beginsOfChromosomes_.empty()) throw "dupa"; //TODO throw an error
        ++secondIterator;
        for(; secondIterator != beginsOfChromosomes_.end();) {
            result.push_back(chromosomeFromGenes(*firstIterator, *secondIterator));
            ++firstIterator;
            ++secondIterator;
        }
        return result;
    }

};

template <typename SpecimenType>
struct SpecimenTraits
{
    using GeneType      = typename SpecimenType::GeneType;
    using GeneContainer = typename SpecimenType::GeneContainer;
};

#endif // __MEMBER__
