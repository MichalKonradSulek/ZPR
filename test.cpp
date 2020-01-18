#include <iostream>
#include <string>
#include "gtest/gtest.h"

#include "Specimen.h"
#include "Mutation.h"
#include "Environment.h"

class CharSpecimen : public Specimen<char, char>
{
public:
    CharSpecimen(const std::string& dna)
    {
        dna_ = dna;
    }
};

class ForcedMutation : public Mutation<char>
{
    int mutationRate = 1000;

public:
    inline bool mutationCondition() final
    {
        return true;
    }

    void mutate(char&& gene)
    {
        gene += 1;
    }
};

const std::string testString("thisIsTestString");

class MySpecimen : public Specimen<char, std::string>
{
public:
    MySpecimen()
    {
        dna_=testString;
    }
};

class MyEnvironment : public Environment<MySpecimen, ForcedMutation>
{
private:
    double fitness(MySpecimen& member)
    {
        return 0;
    }

    inline bool finishCondition() final
    {
        return true;
    }

public:
    MyEnvironment(int populationSize) : Environment(populationSize) { }
};


TEST(specimenTest, getDNA)
{
    const std::string testDNA("qwerty");
    CharSpecimen specimen(testDNA);
    EXPECT_STREQ(specimen.getDNA().data(), testDNA.data());
}

TEST(mutationTest, forcedMutation)
{
    MyEnvironment environment(1);
    environment.mutate();
    std::string result = environment.getBest().getDNA();
    ASSERT_EQ(testString.size(), result.size());
    for(int i = 0; i < testString.size(); ++i) {
        EXPECT_NE(testString.at(i), result.at(i));
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


