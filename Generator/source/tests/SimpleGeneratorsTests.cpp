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

/*TEST (GeneratorRandLevelTest, EqualWithTheSameParametrs) {
  SimpleGenerators firstGenerator = SimpleGenerators(1);
  SimpleGenerators secondGenerator = SimpleGenerators(1);
  SimpleGenerators thirdGenerator = SimpleGenerators(2);
  SimpleGenerators fourthGenerator = SimpleGenerators(2);

  OrientedGraph firstGraph = firstGenerator.generatorRandLevel(1, 3, 1, 3, 3, 3);
  OrientedGraph secondGraph = secondGenerator.generatorRandLevel(1, 3, 1, 3, 3, 3);
  EXPECT_TRUE(firstGraph == secondGraph);
}*/

/*TEST (GeneratorNumOperationsTest, EqualWithTheSameParametrs) {

}*/


