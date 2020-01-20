/**
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
#include <algorithm>
#include <memory>

#include "Specimen.h"

#include "Mutation.h"
#include "Crossover.h"
#include "Selection.h"

#include "Predefined/Mutations.h"
#include "Predefined/Crossovers.h"
#include "Predefined/Selections.h"

#include "Predefined/GAUtility.h"

#include "Exception.h"

namespace GA {

	/**
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

		using Gene			= typename SpecimenType::Gene;
		using Chromosome	= typename SpecimenType::Chromosome;
		using Genotype		= std::vector<Gene>;
		using Fenotype		= std::vector<Chromosome>;

		using Population	= std::vector<SpecimenType>;

	protected:
		Population population_;

		Population mating_pool_;
		Population offspring_;

		std::unique_ptr<Mutation<Gene>>				mutation_type_;
		std::unique_ptr<Crossover<Gene>>			crossover_type_;
		std::unique_ptr<Selection<SpecimenType>>	selection_type_;

	private:
		void setDefaults()
		{
			mutation_type_ = std::make_unique<SwapGeneMutation<Gene> >();
			crossover_type_ = std::make_unique<SinglePointCrossover<Gene> >();
			selection_type_ = std::make_unique<RouletteWheelSelection<SpecimenType> >();
		}

	public:
		explicit Environment(size_type population_size = 0)
		{
			generatePopulation(population_size);

			setDefaults();
		}

		explicit Environment(const Population& population) : population_(population) { setDefaults(); }
		explicit Environment(Population&& population) :		 population_(population) { setDefaults(); }

		explicit Environment(size_type					population_size,
							 Mutation<Gene>*			mutation_ptr,
							 Crossover<Gene>*			crossover_ptr,
							 Selection<SpecimenType>*	selection_ptr)
		{
			generatePopulation(population_size);

			mutation_type_ = std::unique_ptr<Mutation<Gene> >(mutation_ptr);
			crossover_type_ = std::unique_ptr<Crossover<Gene> >(crossover_ptr);
			selection_type_ = std::unique_ptr<Selection<SpecimenType> >(selection_ptr);
		}

		explicit Environment(const Population& population,
			Mutation<Gene>*			mutation_ptr,
			Crossover<Gene>*			crossover_ptr,
			Selection<SpecimenType>*	selection_ptr) : population_(population)
		{
			mutation_type_ = std::unique_ptr<Mutation<Gene> >(mutation_ptr);
			crossover_type_ = std::unique_ptr<Crossover<Gene> >(crossover_ptr);
			selection_type_ = std::unique_ptr<Selection<SpecimenType> >(selection_ptr);
		}

		explicit Environment(Population&& population,
							 Mutation<Gene>*			mutation_ptr,
							 Crossover<Gene>*			crossover_ptr,
							 Selection<SpecimenType>*	selection_ptr) : population_(population)
		{
			mutation_type_ = std::unique_ptr<Mutation<Gene> >(mutation_ptr);
			crossover_type_ = std::unique_ptr<Crossover<Gene> >(crossover_ptr);
			selection_type_ = std::unique_ptr<Selection<SpecimenType> >(selection_ptr);
		}

	protected:
		template <typename FitnessFunction>
		void evaluation(FitnessFunction fitness)
		{
			for (auto& member : population_)
				member.setFitness(fitness(member));
		}

		/**
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

		/**
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
				crossover_type_->cross(mating_pool_[i].getGenotype(), mating_pool_[i + 1].getGenotype());

			offspring_ = std::move(mating_pool_);
		}

		/**
		 *	@brief	Mutation routine, can be overriden to change mutation behaviour
		 *
		 *	@details By default it mutates every indivudual using mutation_type_
		 *
		 *	@note	Mutation should be performed on offspring_
		 */
		virtual void mutation()
		{
			for (auto& individual : offspring_)
				mutation_type_->mutate(individual.getGenotype());
		}

		/**
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

	public:
		/**
		 *	@brief	Evolve by one generation
		 *
		 *	@details Performs one cycle of evolution with given FitnessFunction and FinishCondition
		 *
		 *	@tparam	FitnessFunction	Functor object taking SpecimenType as an argument and returning
		 *			it's fitness value converted to double
		 *
		 *	@param	show_best			 Calls print() on best individual of generation
		 */
		template <typename FitnessFunction>
		void iteration(FitnessFunction fitness, bool show_best = true) {
			selection();

			crossover();
			mutation();

			reproduction();

			evaluation(fitness);

			if (show_best)
				getBest().print();
		}

		/**
		 *	@brief	Perform evolution with given number of generation steps
		 *
		 *	@details Performs genetic algorithm with specified FitnessFunction and
		 *			 FinishCondition.
		 *
		 *	@tparam FitnessFunction	Functor object taking SpecimenType as an argument and returning
		 *			it's fitness value converted to double
		 *	@tparam	FinishCondition	Functor object taking const Population& and FitnessFunction
		 *			and returning a boolean indicator whether a finish condition is met
		 *
		 *	@param	number_of_iterations Specifies a number of generations steps, set to -1 to
		 *			perform evolution until FinishCondition is met
		 *	@param	show_best			 Calls print() on best individual of generation
		 */
		template <typename FitnessFunction, typename FinishCondition>
		void runSimulation(FitnessFunction fitness, FinishCondition finishCondition, int number_of_iterations = -1, bool show_best = true) //TODO dodałbym bool ignoreFinishCondidtions = false
		{
			if (population_.empty())
				generatePopulation(population_.size());
			
			evaluation(fitness);

			if (number_of_iterations == -1)
			{
				while (!finishCondition(population_))
					iteration(fitness, show_best);
			}
			else
			{
				while (!finishCondition(population_) && --number_of_iterations >= 0)
					iteration(fitness, show_best);
			}
		}

		SpecimenType& getBest()
		{
			SpecimenComp<SpecimenType> comp;

			auto it = std::max_element(population_.begin(), population_.end(), comp);
			return *it;
		}

		/**
		 *	@brief	Generates new population and replaces currently held one
		 */
		virtual void generatePopulation(size_t population_size)
		{
			population_.clear();
			population_.reserve(population_size);

			for (size_type i = 0; i < population_size; ++i)
				population_.emplace_back(SpecimenType());
		}

		//	Get/set population
		void setPopulation(const Population& population)
		{
			population_ = population;
		}

		void setPopulation(Population&& population)
		{
			population_ = population;
		}

		const Population& getPopulation() const
		{
			return population_;
		}

		//	For Strategies with strictly specified Gene/Specimen type
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

		//	Generic function templates taking a specified strategy
		template <template <typename T> typename MutationType, typename... Args>
		void setMutationType(Args&&... args)
		{
			mutation_type_.reset();
			mutation_type_ = std::make_unique<MutationType<Gene> >(std::forward<Args>(args)...);
		}

		template <template <typename T> typename CrossoverType, typename... Args>
		void setCrossoverType(Args&&... args)
		{
			crossover_type_.reset();
			crossover_type_ = std::make_unique<CrossoverType<Gene> >(std::forward<Args>(args)...);
		}

		template <template <typename T> typename SelectionType, typename... Args>
		void setSelectionType(Args&&... args)
		{
			selection_type_.reset();
			selection_type_ = std::make_unique<SelectionType<SpecimenType> >(std::forward<Args>(args)...);
		}
	};

}

#endif // __ENVIRONMENT__