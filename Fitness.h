//
// Created by michalsulek on 16.01.2020.
//

#ifndef ZPR_FITNESS_H
#define ZPR_FITNESS_H

#include "Member.h"

template <typename SpecimenType>
class Fitness {
protected:
    using Subject      = SpecimenType;
public:
    virtual ~Fitness() = default;
    virtual double rateSpecimen (const Subject& specimen, const std::vector<Subject>& population) const {return 0;};
};

#endif //ZPR_FITNESS_H