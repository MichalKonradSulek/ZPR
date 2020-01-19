/*
 *  Class defining environment for genetic algorithm. After setting up
 *  environment settings one function performs whole algorithm with
 *  specified loop (generation happens only once at the beginning):
 *      generate -> evaluate -> selection -> genetic_operators -> succession
 *
 *  Authors: Michal Swiatek, Michal Sulek
 *	Update:	 19.01.2020
 *
 *	Github repository: https://github.com/MichalKonradSulek/ZPR
 */

#ifndef __ENVIRONMENT__
#define __ENVIRONMENT__

#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <memory>

#include "Specimen.h"

#include "Mutation.h"
#include "Crossover.h"
#include "Selection.h"

#include "Predefined.h"

namespace GA {

	/*
	 *	@brief	An environment defining population and specified genetic operators
	 *
	 *	@details This class sets all genetic operators, selection strategy and generates
	 *			 population of Specimens. It then performs a standard genetic algorithm
	 *			 using specified fitness function and finish condition.
	 *
	 *	@tparam	SpecimenType Type of a member of population
	 *
	 *	@see	Specimen
	 *	@see	Mutation
	 *	@see	Crossover
	 *	@see	Selection
	 */
	template <typename SpecimenType>
	class Environment
	{
	public:
		using size_type = size_t;

		using Gene = typename SpecimenType::Gene;
		using Chromosome = typename SpecimenType::Chromosome;
		using Genotype = std::vector<Gene>;
		using Fenotype = std::vector<Chromosome>;

	protected:
		std::vector<SpecimenType> population_;

		std::vector<SpecimenType> mating_pool_;
		std::vector<SpecimenType> offspring_;

		std::unique_ptr<Mutation<Gene>>				mutation_type_;
		std::unique_ptr<Crossover<Gene>>			crossover_type_;
		std::unique_ptr<Selection<SpecimenType>>	selection_type_;

		virtual bool finishCondition()
		{
			return false;
		}

	public:
		explicit Environment(int population_size)
		{
			population_.resize(population_size);

			mutation_type_ = std::make_unique<SwapGeneMutation<Gene> >(); //TODO dodałbym do argumentów pointery na wszystkie rzeczy z ustawionym domyślnym new ...
			crossover_type_ = std::make_unique<SinglePointCrossover<Gene> >();
			selection_type_ = std::make_unique<BestFitnessSelection<SpecimenType> >();
		}

		virtual void setPopulation(size_type population_size)
		{
			population_.clear();
			population_.assign(population_size, SpecimenType());
		}

		template <typename FitnessFunction>
		void evaluation(FitnessFunction fitness)
		{
			for (auto& member : population_)
				member.setFitness(fitness(member));
		}

		/*
		 *	@brief  Selection routine, can be overriden to change selection behaviour
		 *	
		 *	@details By default this function performs selection of population based on selection_type_
		 *
		 *	@note	This function has to generate and assign new mating_pool_
		 *	@note	During this function call current population_ is already evaluated
		 */
		virtual void selection()
		{
			mating_pool_ = selection_type_->select(population_, population_.size());
		}

		/*
		 *	@brief	Crossover routine, can be overriden to change crossover behavior
		 *
		 *	@details By default it crosses adjacent members (they are randomly placed by selection)
		 *			 using crossover_type_
		 *
		 *	@note	This function has to assign new offspring_
		 */
		virtual void crossover()
		{
			for (size_t i = 0; i < mating_pool_.size() - 1; i += 2)
				crossover_type_->cross(mating_pool_[i].getDNA(), mating_pool_[i + 1].getDNA());

			offspring_ = std::move(mating_pool_);
		}

		/*
		 *	@brief	Mutation routine, can be overriden to change mutation behaviour
		 *
		 *	@details By default it mutates every indivudual using mutation_type_
		 *
		 *	@note	Mutation should be performed on offspring_
		 */
		virtual void mutation()
		{
			for (auto& individual : offspring_)
				mutation_type_->mutate(individual.getDNA());
		}

		/*
		 *	@brief	Reproduction routine, can be overriden to change reproduction behavior
		 *
		 *	@details By default it moves an offspring_ into population_
		 *
		 *	@note	This function has to assign new population_
		 */
		virtual void reproduction()
		{
			population_ = std::move(offspring_);
		}

		/*
		 *	@brief	Evolve by one generation
		 *
		 *	@details Performs one cycle of evolution with given FitnessFunction and FinishCondition
		 *
		 *	@tparam	FitnessFunction	Functor object taking SpecimenType as an argument and returning
		 *			it's fitness value converted to double
		 */
		template <typename FitnessFunction>
		void iteration(FitnessFunction fitness) {
			evaluation(fitness);

			//	TODO: remove this line, if sorting is needed selection should perform it
			std::sort(population_.begin(), population_.end(), [](SpecimenType& a, SpecimenType& b) {return a.getFitness() > b.getFitness(); });

			selection();

			crossover();
			mutation();

			reproduction();

			showBest();		//	TODO: remove
		}

		/*
		 *	@brief	Perform evolution with given number of generation steps
		 *
		 *	@details Performs genetic algorithm with specified FitnessFunction and
		 *			 FinishCondition.
		 *
		 *	@tparam FitnessFunction	Functor object taking SpecimenType as an argument and returning
		 *			it's fitness value converted to double
		 *
		 *	@param	number_of_iterations Specifies a number of generations steps, set to -1 to
		 *			perform evolution until FinishCondition is met
		 */
		template <typename FitnessFunction>
		void runSimulation(FitnessFunction fitness, int number_of_iterations = -1) //TODO dodałbym bool ignoreFinishCondidtions = false
		{
			setPopulation(population_.size());

			if (number_of_iterations == -1)
			{
				while (!finishCondition())
					iteration(fitness);
			}
			else
			{
				while (!finishCondition() && --number_of_iterations >= 0)
					iteration(fitness);
			}
		}

		void showBest()
		{
			auto fenotype = population_[0].getFenotype();
			std::cout << std::string(fenotype.begin(), fenotype.end()) << "\tfitness: " << population_[0].getFitness() << '\n'; //TODO dangerous population_[0]
		}

		SpecimenType getBest()
		{
			if (!population_.size())	throw std::exception("Population vector is empty!"); //TODO implement exceptionsS
			return population_[0];
		}

		template <typename MutationType, typename... Args>
		void setMutationType(Args&&... args)
		{
			mutation_type_.reset();
			mutation_type_ = std::make_unique<MutationType>(std::forward<Args>(args)...);
		}

		template <typename CrossoverType, typename... Args>
		void setCrossoverType(Args&&... args)
		{
			crossover_type_.reset();
			crossover_type_ = std::make_unique<CrossoverType>(std::forward<Args>(args)...);
		}

		template <typename SelectionType, typename... Args>
		void setSelectionType(Args&&... args)
		{
			selection_type_.reset();
			selection_type_ = std::make_unique<SelectionType>(std::forward<Args>(args)...);
		}
	};

}

#endif // __ENVIRONMENT__
