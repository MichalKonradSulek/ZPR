/**
 *	Class representing member of population, GeneContainer specified by user
 *	has been removed due to vector reliability and efficiency. Continous storage
 *	supplied by vector is best suiting for keeping DNA data.
 *	GeneType is used to build DNA of Specimen, but fitness evaluation is based on
 *	ChromosomeType generated from that DNA.
 *	
 *	Authors: Michal Swiatek, Michal Sulek
 *	Update:	 18.01.2020
 *
 *	Github repository: https://github.com/MichalKonradSulek/ZPR
 */

#ifndef __SPECIMEN__
#define __SPECIMEN__

#include <vector>
#include <iostream>

namespace ga {

	/**
	 *	@brief Class defining single member of population
	 *
	 *	@details Specimen represents genetic data used to determine environmental
	 *			 fitness of a member
	 *
	 *	@tparam GeneType		Used to build DNA of Specimen
	 *	@tparam ChromosomeType	Used to evaluate Specimen trough fitness function
	 */
	template <typename GeneType, typename ChromosomeType>
	class Specimen
	{
	public:
		using Gene = GeneType;
		using Chromosome = ChromosomeType;
		using Genotype = std::vector<Gene>;
		using Fenotype = std::vector<Chromosome>;

	private:
		double fitness_;

	protected:
		Genotype dna_;

	public:
		Specimen() : fitness_(0.0) { }
		explicit Specimen(const Genotype& genotype) : dna_(genotype), fitness_(0.0) {}
		explicit Specimen(Genotype&& genotype) : dna_(genotype), fitness_(0.0) {}
		virtual ~Specimen() = default;

		/**
		 *	@brief Conversion of Genotype into Fenotype
		 *	
		 *	@details Define Chromosome conversion when using Chromosome different from Gene
		 *			 otherwise just return dna_
		 */
		virtual Fenotype getFenotype() const = 0;

		virtual void print() const
		{
			std::cout << fitness_ << '\n';
		}

		virtual Genotype& getGenotype() { return dna_; }

		inline double	getFitness() const { return fitness_; }
		inline void		setFitness(double fitness) { fitness_ = fitness; }
	};

}

#endif // __SPECIMEN__
