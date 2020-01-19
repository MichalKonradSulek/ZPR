#include <iostream>
#include <string>
#include <gtest/gtest.h>

#include "Specimen.h"
#include "Mutation.h"
#include "Environment.h"

class testSpecimen : public GA::Specimen<bool, bool> {
public:
    explicit testSpecimen(const Genotype& genotype) : GA::Specimen<bool, bool>(genotype) {}
    Fenotype getFenotype() const override {
        return dna_;
    }
};

//class ForcedMutation : public Mutation<char>
//{
//    int mutationRate = 1000;
//
//public:
//    inline bool mutationCondition() final
//    {
//        return true;
//    }
//
//    void mutate(char&& gene)
//    {
//        gene += 1;
//    }
//};
//
//const std::string testString("thisIsTestString");
//
//class MySpecimen : public Specimen<char, std::string>
//{
//public:
//    MySpecimen()
//    {
//        dna_=testString;
//    }
//};
//
//class MyEnvironment : public Environment<MySpecimen, ForcedMutation>
//{
//private:
//    double fitness(MySpecimen& member)
//    {
//        return 0;
//    }
//
//    inline bool finishCondition() final
//    {
//        return true;
//    }
//
//public:
//    MyEnvironment(int populationSize) : Environment(populationSize) { }
//};


TEST(specimenTest, getDNA)
{
    const std::vector<bool> testDNA{1,1,1,1,0,0,0,0};
    testSpecimen specimen(testDNA);
    EXPECT_EQ(specimen.getGenotype(), testDNA);
}

//TEST(mutationTest, forcedMutation)
//{
//    MyEnvironment environment(1);
//    environment.mutate();
//    std::string result = environment.getBest().getDNA();
//    ASSERT_EQ(testString.size(), result.size());
//    for(int i = 0; i < testString.size(); ++i) {
//        EXPECT_NE(testString.at(i), result.at(i));
//    }
//}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


