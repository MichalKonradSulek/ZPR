//
// Created by michalsulek on 16.01.2020.
//

/*
 *  Class encapsulating fitness function into object
 *	Fitness function is used to evaluate how an individual
 *	is suited for particular Environment
 *
 *  Authors: Michal Sulek
 *	Update:	 18.01.2020
 *
 *	Github repository: https://github.com/MichalKonradSulek/ZPR
 *
 *	TODO: Remove this class, use std::function or templates
 */

#ifndef __FITNESS__
#define __FITNESS__

#include "Specimen.h"

template <typename SpecimenType>
class Fitness {
protected:
    using Subject      = SpecimenType;
public:
    virtual ~Fitness() = default;
    virtual double rateSpecimen (const Subject& specimen, const std::vector<Subject>& population) const {return 0;};
};

#endif //__FITNESS__