#include <gtest/gtest.h>
#include "../generators/SimpleGenerators.h"

TEST (CnfFromTruthTableTest, EqualWithTheSameParametrs) {
  SimpleGenerators firstGenerator = SimpleGenerators(1);
  SimpleGenerators secondGenerator = SimpleGenerators(1);
  SimpleGenerators thirdGenerator = SimpleGenerators(2);
  SimpleGenerators fourthGenerator = SimpleGenerators(2);
    
  //Test case 1
  TruthTable table = TruthTable{ 1,  1 , std::vector<std::vector<bool>>({{1},{1}}) };
  std::vector<std::string> firstCnf = firstGenerator.cnfFromTruthTable(table, true);
  std::vector<std::string> secondCnf = secondGenerator.cnfFromTruthTable(table, true);
  EXPECT_TRUE(firstCnf == secondCnf);
  firstCnf = thirdGenerator.cnfFromTruthTable(table, true);
  secondCnf = fourthGenerator.cnfFromTruthTable(table, true);
  EXPECT_TRUE(firstCnf == secondCnf);

  //Test case 2
  table = TruthTable{ 2,  1 , std::vector<std::vector<bool>>({{1},{1} ,{1} , {0}}) };
  firstCnf = firstGenerator.cnfFromTruthTable(table, true);
  secondCnf = secondGenerator.cnfFromTruthTable(table, true);
  EXPECT_TRUE(firstCnf == secondCnf);
  firstCnf = thirdGenerator.cnfFromTruthTable(table, true);
  secondCnf = fourthGenerator.cnfFromTruthTable(table, true);
  EXPECT_TRUE(firstCnf == secondCnf);

  //Test case 3
  table = TruthTable{ 2,  2 , std::vector<std::vector<bool>>({{1 , 1},{1 , 1} ,{1 , 1} , {0 , 0}}) };
  firstCnf = firstGenerator.cnfFromTruthTable(table, true);
  secondCnf = secondGenerator.cnfFromTruthTable(table, true);
  EXPECT_TRUE(firstCnf == secondCnf);
  firstCnf = thirdGenerator.cnfFromTruthTable(table, true);
  secondCnf = fourthGenerator.cnfFromTruthTable(table, true);
  EXPECT_TRUE(firstCnf == secondCnf);
}

TEST (GeneratorRandLevelTest, EqualWithTheSameParametrs) {
  SimpleGenerators firstGenerator = SimpleGenerators(1);
  SimpleGenerators secondGenerator = SimpleGenerators(1);
  SimpleGenerators thirdGenerator = SimpleGenerators(2);
  SimpleGenerators fourthGenerator = SimpleGenerators(2);

  OrientedGraph firstGraph = firstGenerator.generatorRandLevel(1, 3, 1, 3, 3, 3);
  OrientedGraph secondGraph = secondGenerator.generatorRandLevel(1, 3, 1, 3, 3, 3);
  EXPECT_TRUE(firstGraph == secondGraph);
  firstGraph = thirdGenerator.generatorRandLevel(1, 3, 1, 3, 3, 3);
  secondGraph = fourthGenerator.generatorRandLevel(1, 3, 1, 3, 3, 3);
  EXPECT_TRUE(firstGraph == secondGraph);

  firstGraph = firstGenerator.generatorRandLevel(2, 6, 2, 6, 4, 4);
  secondGraph = secondGenerator.generatorRandLevel(2, 6, 2, 6, 4, 4);
  EXPECT_TRUE(firstGraph == secondGraph);
  firstGraph = thirdGenerator.generatorRandLevel(2, 6, 2, 6, 4, 4);
  secondGraph = fourthGenerator.generatorRandLevel(2, 6, 2, 6, 4, 4);
  EXPECT_TRUE(firstGraph == secondGraph);
}

/*TEST (GeneratorRandLevelExperimentalTest, EqualWithTheSameParametrs) {
  SimpleGenerators firstGenerator = SimpleGenerators(1);
  SimpleGenerators secondGenerator = SimpleGenerators(1);
  SimpleGenerators thirdGenerator = SimpleGenerators(2);
  SimpleGenerators fourthGenerator = SimpleGenerators(2);

  OrientedGraph firstGraph = firstGenerator.generatorRandLevelExperimental(1, 3, 1, 3, 3, 3);
  OrientedGraph secondGraph = secondGenerator.generatorRandLevelExperimental(1, 3, 1, 3, 3, 3);
  EXPECT_TRUE(firstGraph == secondGraph);
  firstGraph = thirdGenerator.generatorRandLevelExperimental(1, 3, 1, 3, 3, 3);
  secondGraph = fourthGenerator.generatorRandLevelExperimental(1, 3, 1, 3, 3, 3);
  EXPECT_TRUE(firstGraph == secondGraph);

  firstGraph = firstGenerator.generatorRandLevelExperimental(2, 6, 2, 6, 4, 4);
  secondGraph = secondGenerator.generatorRandLevelExperimental(2, 6, 2, 6, 4, 4);
  EXPECT_TRUE(firstGraph == secondGraph);
  firstGraph = thirdGenerator.generatorRandLevelExperimental(2, 6, 2, 6, 4, 4);
  secondGraph = fourthGenerator.generatorRandLevelExperimental(2, 6, 2, 6, 4, 4);
  EXPECT_TRUE(firstGraph == secondGraph);
}*/

TEST (GeneratorNumOperationsTest, EqualWithTheSameParametrs) {
  SimpleGenerators firstGenerator = SimpleGenerators(1);
  SimpleGenerators secondGenerator = SimpleGenerators(1);
  SimpleGenerators thirdGenerator = SimpleGenerators(2);
  SimpleGenerators fourthGenerator = SimpleGenerators(2);
  
  std::map<std::string, int> logicOper = {
    {"and", 2},
    {"or", 2},
    {"not", 1}
  };
  OrientedGraph firstGraph = firstGenerator.generatorNumOperation(1, 1, logicOper, true);
  OrientedGraph secondGraph = secondGenerator.generatorNumOperation(1, 1, logicOper, true);   
  EXPECT_TRUE(firstGraph == secondGraph);
  firstGraph = thirdGenerator.generatorNumOperation(1, 1, logicOper, true);
  secondGraph = fourthGenerator.generatorNumOperation(1, 1, logicOper, true);   
  EXPECT_TRUE(firstGraph == secondGraph);

  logicOper = {
    {"and", 5},
    {"or", 5},
    {"not", 1},
    {"nor", 1},
    {"nand", 1},
    {"xnor", 1},
    {"xor", 1}
  };

  firstGraph = firstGenerator.generatorNumOperation(1, 1, logicOper, true);
  secondGraph = secondGenerator.generatorNumOperation(1, 1, logicOper, true);
  EXPECT_TRUE(firstGraph == secondGraph);
  firstGraph = thirdGenerator.generatorNumOperation(1, 1, logicOper, true);
  secondGraph = fourthGenerator.generatorNumOperation(1, 1, logicOper, true);   
  EXPECT_TRUE(firstGraph == secondGraph);

  firstGraph = firstGenerator.generatorNumOperation(5, 10, logicOper, true);
  secondGraph = secondGenerator.generatorNumOperation(5, 10, logicOper, true);
  EXPECT_TRUE(firstGraph == secondGraph);
  firstGraph = thirdGenerator.generatorNumOperation(5, 10, logicOper, true);
  secondGraph = fourthGenerator.generatorNumOperation(5, 10, logicOper, true);   
  EXPECT_TRUE(firstGraph == secondGraph);

  logicOper = {
    {"and", 5},
    {"or", 5},
    {"not", 6},
    {"nor", 6},
    {"nand", 1},
    {"xnor", 9},
    {"xor", 12}
  };

  firstGraph = firstGenerator.generatorNumOperation(1, 1, logicOper, false);
  secondGraph = secondGenerator.generatorNumOperation(1, 1, logicOper, false);
  EXPECT_TRUE(firstGraph == secondGraph);
  firstGraph = thirdGenerator.generatorNumOperation(1, 1, logicOper, false);
  secondGraph = fourthGenerator.generatorNumOperation(1, 1, logicOper, false);   
  EXPECT_TRUE(firstGraph == secondGraph);

  firstGraph = firstGenerator.generatorNumOperation(7, 9, logicOper, false);
  secondGraph = secondGenerator.generatorNumOperation(7, 9, logicOper, false);
  EXPECT_TRUE(firstGraph == secondGraph);
  firstGraph = thirdGenerator.generatorNumOperation(7, 9, logicOper, false);
  secondGraph = fourthGenerator.generatorNumOperation(7, 9, logicOper, false);   
  EXPECT_TRUE(firstGraph == secondGraph);
}



