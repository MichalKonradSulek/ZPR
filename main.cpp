#include <iostream>
#include <cstdlib>
#include <ctime>

#include "Test1.h"
#include "Test3.h"

#include "Fitness.h"

int main()
{
    srand(time(nullptr));

    MyEnvironment  env1(10000);
    Environment<MySpecimen3>  env3(10000);

	auto fitness1 = [](const MySpecimen& specimen)
	{
		double result = 0;
		const auto& fenotype = specimen.getFenotype();

		for (size_t i = 0; i < str1.size(); i++)
		{
		    if (fenotype[i] == str1[i])
		        result += 10;
		    else if (fenotype[i] < 32 || fenotype[i] > 126)
		        result -= 5;
		}

		return result;
	};

	env1.setMutationType<FlipBitMutation>(10, GENES_PER_CHROMOSOME * str1.length());
	env1.setSelectionType<BestFitnessSelection<MySpecimen> >(5);

    env1.runSimulation(fitness1, 100);

	auto fitness3 = [](const MySpecimen3& specimen)
	{
		double result = 0;
		const auto& fenotype = specimen.getFenotype();

		for (size_t i = 0; i < str3.size(); i++)
		{
			if (fenotype[i] == str3[i])
				result += 10;
			else
				result -= 5;
		}

		return result;
	};

	env3.setMutationType<CharMutation>(50, str3.length());

	env3.runSimulation(fitness3, 30);

    std::cout << "\n\n\n";

    return 0;
}
