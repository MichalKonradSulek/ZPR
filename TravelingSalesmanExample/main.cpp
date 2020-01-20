#include "Include.hpp"

int main() {
	srand(time(nullptr));

	//	Define cities
	std::array<std::pair<int, int>, NUMBER_OF_CITIES> cities;
	for (size_t i = 0; i < NUMBER_OF_CITIES; ++i)
		cities[i] = std::make_pair(i, i);	//	Generate predictable solution
		//cities[i] = std::make_pair(rand() % DIMENSIONS, rand() % DIMENSIONS);

	GA::Environment<Specimen> env(500);

	env.setMutationType<GA::SwapGeneMutation>(-1, GA::MUTATION_CHANCE_PERCENT * 10, 5);
	env.setCrossoverType<UniqueCrossover>();
	env.setSelectionType<GA::StochasticUniversalSamplingSelection>();

	auto fitness = [&cities](const Specimen& specimen) -> double
	{
		auto fenotype = specimen.getFenotype();

		//	Calculate total distance
		double dist = 0.0;
		for (size_t i = 0; i < fenotype.size() - 1; ++i)
		{
			auto city1 = cities[fenotype[i    ]];
			auto city2 = cities[fenotype[i + 1]];

			int x_diff = city1.first - city2.first;
			int y_diff = city1.second - city2.second;

			dist += x_diff * x_diff + y_diff * y_diff;
		}

		return 1000.0 / (dist + 1);
	};

	auto finishCondition = [](const auto& population)
	{
		GA::SpecimenComp<Specimen> comp;

		auto best = std::max_element(population.begin(), population.end(), comp);

		auto best_fenotype = (*best).getFenotype();

		int counter1 = 0, counter2 = 0;
		for (int i = 0; i < NUMBER_OF_CITIES; ++i)
		{
			if (best_fenotype[i] == i)
				++counter1;

			if (best_fenotype[i] == NUMBER_OF_CITIES - 1 - i)
				++counter2;
		}

		if (counter1 == NUMBER_OF_CITIES || counter2 == NUMBER_OF_CITIES)
			return true;
		else
			return false;
	};

	env.runSimulation(fitness, finishCondition, -1, false);

	auto best = env.getBest();

	for (const auto& city : best.getFenotype())
		std::cout << city << ' ';

	std::cout << '\n';

	return 0;
}
