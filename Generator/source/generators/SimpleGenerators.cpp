#include <algorithm>
#include <ctime>
#include <iostream>
#include <math.h>
#include <bitset>

#include "SimpleGenerators.h"
#include "../graph/OrientedGraph.h"
#include "Parser.h"
#include "TruthTable.h"



namespace {
  int maxValueInMap(const std::map<std::string, int>& i_map)
  {
    if (i_map.size() == 0)
    {
      return -1;
    }
    int res = (*i_map.begin()).second;

    for (const auto &[key, value] : i_map)
    {
      res = std::max(res, value);
    }
    return res;
  }
}

SimpleGenerators::SimpleGenerators()
{
  std::srand(std::time(0));
}

std::vector<std::string> SimpleGenerators::cnfFromTruthTable(
  const TruthTable& i_table, 
  bool i_tp
)
{
  std::vector<std::string> fun;
  std::vector<std::vector<bool>> bin = i_table.convToBinary();

  for (int j = 0; j < i_table.getOutput(); ++j)
  {
    fun.push_back("f" + std::to_string(j) + " = ");
    int mem = 0;
    int tmp = 0;

    for (int i = 0; i < i_table.size(); ++i)
    {
      if (!(i_table.getOutTable(i, j) ^ i_tp))
        mem++;
    }

    if (mem == 0)
    {
      if (i_tp)
        fun[j] += "1'b0";
      else
        fun[j] += "1'b1";
      continue;
    }

    if (mem == i_table.size())
    {
      if (i_tp)
        fun[j] += "1'b1";
      else
        fun[j] += "1'b0";
      continue;
    }

    for (int i = 0; i < mem; ++i)
    {
      fun[j] += '(';
      while ((i_table.getOutTable(tmp, j) ^ i_tp) && tmp < i_table.size())
        tmp++;

      for (int k = 0; k < i_table.getInput(); ++k)
      {
        if (bin[tmp][k] ^ i_tp)
          fun[j] += d_settings->getLogicOperation("not").first + " ";

        fun[j] += 'x';
        fun[j] += std::to_string(k);

        if (k != i_table.getInput() - 1)
          fun[j] += " " + (i_tp ? 
                           d_settings->getLogicOperation("and").first : 
                           d_settings->getLogicOperation("or").first
                        ) + " ";
      }

      fun[j] += ')';

      if (i != mem - 1)
        fun[j] += " " + (i_tp ? 
                       d_settings->getLogicOperation("or").first : 
                       d_settings->getLogicOperation("and").first
                      ) + " ";

      tmp++;
    }
  }

  return fun;
}

std::vector<std::string> SimpleGenerators::zhegalkinFromTruthTable(const TruthTable& i_table) 
{
    std::vector<std::string> polynomial;
    int num_inputs = i_table.getInput();
    int num_outputs = i_table.getOutput();
    int num_terms = std::pow(2, num_inputs);

    for (int j = 0; j < num_outputs; ++j) {
        std::vector<int> coefficients(num_terms, 0);

        
        for (int i = 0; i < i_table.size(); ++i) {
            if (i_table.getOutTable(i, j)) {
                coefficients[i] = 1;
            }
        }

        std::string poly = "f" + std::to_string(j) + " = ";
        bool first_term = true;
        for (int term = 0; term < num_terms; ++term) {
            if (coefficients[term] != 0) {
                if (!first_term) {
                    poly += " " + d_settings->getLogicOperation("xor").first + " ";
                }
                if (term == 0) {
                    poly += "1";
                } else {
                    poly += "(";
                    for (int i = 0; i < num_inputs; ++i) {
                        if ((term >> (num_inputs - i - 1)) & 1) {
                            poly += "x" + std::to_string(i);
                        } else {
                            poly +=  d_settings->getLogicOperation("not").first + " x" + std::to_string(i);
                        }
                        if (i != num_inputs - 1) {
                            poly += " " + d_settings->getLogicOperation("and").first + " ";
                        }
                    }
                    poly += ")";
                }
                first_term = false;
            }
        }
        polynomial.push_back(poly);
    }
    return polynomial;  
}

OrientedGraph SimpleGenerators::generatorRandLevel(int i_maxLevel, int i_maxElements, int i_inputs, int i_outputs)
{
  int maxLevel;
  if (i_maxLevel != 0) maxLevel = rand() % i_maxLevel + 2; // TODO: Zunin , not +1?
  else maxLevel = 2;
  std::vector<int> elemLevel(maxLevel + 1);
  std::vector<std::string> logOper = d_settings->getLogicOperationsKeys();
  logOper.erase(std::find(logOper.begin(), logOper.end(), "input"));
  logOper.erase(std::find(logOper.begin(), logOper.end(), "output"));
  logOper.erase(std::find(logOper.begin(), logOper.end(), "const"));

  //maxLevel++ // what?
  elemLevel[0] = i_inputs;
  elemLevel[maxLevel] = i_outputs;
  for (int i = 1; i < maxLevel; ++i)
  { 
      if (i_maxElements > 1) elemLevel[i] = (rand() % (i_maxElements - 1)) + 2;
      else elemLevel[i] = 2;
  }

  int choice;
  std::string expr;
  OrientedGraph graph;
  int child1, child2;

  for (int i = 0; i < elemLevel[0]; ++i)
  {
    expr = "x" + std::to_string(i);
    graph.addVertex(expr, "input");
  }

  int currIndex = elemLevel[0];
  int prevIndex = 0;

  for (int i = 1; i < maxLevel; ++i)
  {
    int position = 0;
    for (int j = 0; j < elemLevel[i]; ++j)
    {
      choice = rand() % logOper.size();
      if (logOper[choice] == "not")
      {
        child1 = rand() % currIndex;
        expr = d_settings->fromOperationsToName(logOper[choice]) + " (" +
          graph.getVertice(child1).getLogicExpression() + ")";

        if (graph.addVertex(expr, "not"))
          graph.addEdge(graph.getVertice(child1).getLogicExpression(), graph.getVertice(currIndex + position).getLogicExpression());
        else
          --position;
      }
      else
      {
        child1 = (rand() % (currIndex - prevIndex)) + prevIndex;
        child2 = (rand() % (currIndex - prevIndex)) + prevIndex;

        expr = "(" + graph.getVertice(child2).getLogicExpression() + " )" +
          d_settings->fromOperationsToName(logOper[choice]) + " (" + graph.getVertice(child1).getLogicExpression() + ")";

        if (graph.addVertex(expr, logOper[choice]))
          graph.addDoubleEdge(graph.getVertice(child2).getLogicExpression(),
                              graph.getVertice(child1).getLogicExpression(),
                              graph.getVertice(currIndex + position).getLogicExpression());
        else
          --position;
      }
      ++position;
    }

    prevIndex += currIndex - prevIndex;
    currIndex += position;
  }

  //TODO: fix when elements less than outputs

  for (int i = 0; i < i_outputs; ++i)
  {
    child1 = (rand() % (currIndex - prevIndex)) + prevIndex;
    expr = "f" + std::to_string(i + 1);
    graph.addVertex(expr, "output");
    graph.addEdge(graph.getVertice(child1).getLogicExpression(),
                  graph.getVertice(currIndex + i).getLogicExpression()
    );
  }
  return graph;
}

OrientedGraph SimpleGenerators::generatorNumOperation(
  int i_input,
  int i_output,
  std::map<std::string, int> i_logicOper,
  bool i_leaveEmptyOut
)
{
  int sumOper = 0, maxLvl;
  std::string name;
  std::map<std::string, int> copyLogicOper, levelName;
  std::vector<std::string> nameOut, nameInput;

  for(const auto& elem : i_logicOper)
  {
    std::cout << elem.first << " " << elem.second << "\n";
  }

  copyLogicOper = i_logicOper;
  
  OrientedGraph graph;

  for (int i = 0; i < i_input; ++i)
  {
    name = "x" + std::to_string(i);
    graph.addVertex(name, "input");
    levelName[name] = graph.getVertice(graph.getIndexOfExpression(name)).getLevel();
    if (!i_leaveEmptyOut)
      nameInput.push_back(name);
  }

  for (int i = 0; i < i_output; ++i)
  {
    name = "f" + std::to_string(i);
    graph.addVertex(name, "output");
    nameOut.push_back(name);
  }

  for (const auto &[key, value] : copyLogicOper)
    sumOper += value;

  for (int i = 0; i < sumOper; ++i)
  {
    copyLogicOper = delNull(copyLogicOper); // TODO: optimize
    std::string oper = randomGenerator(copyLogicOper);
    copyLogicOper[oper]--;

    if (oper == "not")
    {
      std::string ver1 = randomGenerator(levelName);
      name = d_settings->getLogicOperation(oper).first + "(" + ver1 + ")";
      if (graph.addVertex(name, oper))
      {
        graph.addEdge(ver1, name);
      }
      else
      {
        copyLogicOper[oper]++;
        sumOper++;
      }
    }
    else
    {
      std::string ver1 = randomGenerator(levelName);
      std::string ver2 = randomGenerator(levelName);
      name = "(" + ver1 + ") " + d_settings->getLogicOperation(oper).first + "(" + ver2 + ")";
      std::string reserveName = "(" + ver2 + ") " + d_settings->getLogicOperation(oper).first + "(" + ver1 + ")";
      if (graph.addVertex(name, oper))
      {
        graph.addDoubleEdge(ver1, ver2, name);
        levelName[name] = graph.getVertice(graph.getIndexOfExpression(name)).getLevel();
      }
      else
      {
        copyLogicOper[oper]++;
        sumOper++;
      }
    }
  }

  while ((nameOut.size() > 0) & ((levelName.size() > 0 || i_leaveEmptyOut == false)))
  {
    if (levelName.size() > 0)
    {
      std::vector<std::string> help;
      maxLvl = maxValueInMap(levelName);
      
      for (const auto &[key, value] : levelName)
      {
        if (value == maxLvl)
          help.push_back(key);
      }

      while (help.size() > 0 && nameOut.size() > 0)
      {
        int R1 = rand() % help.size();
        int R2 = rand() % nameOut.size();
        graph.addEdge(help[R1], nameOut[R2]);
        levelName.erase(help[R1]);
        help.erase(help.begin() + R1);
        nameOut.erase(nameOut.begin() + R2);
      }
    }
    else
    {
      int R1 = rand() % nameInput.size();
      int R2 = rand() % nameOut.size();

      graph.addEdge(nameInput[R1], nameOut[R2]);
      nameOut.erase(nameOut.begin() + R2);
    }
  }
  return graph;
}

std::map<std::string, int> SimpleGenerators::delNull(std::map<std::string, int> i_copyLogicOper)
{
  std::vector<std::string> delList;
  for (const auto &[key, value] : i_copyLogicOper)
    if (value == 0)
      delList.push_back(key);

  for (const auto& op : delList)
    i_copyLogicOper.erase(op);
  return i_copyLogicOper;
}

std::string SimpleGenerators::randomGenerator(const std::map<std::string, int>& i_map)
{
  int i = rand() % i_map.size();

  auto p = i_map.begin();

  while(i--)
    p++;

  return (*p).first;

}

OrientedGraph SimpleGenerators::generatorSummator(int i_bits, bool i_overflowIn, bool i_overflowOut, bool i_minus, bool act)
{
    OrientedGraph graph;
    if (i_overflowIn)
        graph.addVertex("x_p0", "input");
    if (act)
        graph.addVertex("1", "const");
    std::string pi;
    std::string x;
    std::string y;
    std::string z;
    std::string cond = std::string(i_overflowIn ? "t" : "f") + (i_overflowOut ? "t" : "f") + (i_minus ? "t" : "f");
    z = std::string(i_minus ? "n" : "") + "s" + (!i_overflowIn && !i_overflowOut ? "0" : (!i_overflowIn && i_overflowOut ? "1" : (i_overflowIn && !i_overflowOut ? "2" : "3")));
    for (int i = 0; i < i_bits; i++){
        std::string S = std::to_string(i);
        x = "x_suma" + cond + S;
        y = "x_sumb" + cond + S;
        graph.addVertex(x, "input");
        graph.addVertex(y, "input");
        if (i_minus)
        {
            graph.addVertex("not (" + x + ")", "not", "na" + S);
            graph.addVertex("not (" + y + ")", "not", "nb" + S);
            graph.addEdge(x, "na" + S, false);
            graph.addEdge(y, "nb" + S, false);
            x = "na" + S;
            y = "nb" + S;
        }
        if (!act)
        {
            graph.addVertex(z + S, "output");
        }

        graph.addVertex("(" + x + " and " + y + ")", "and", "andab" + S);

        std::string NextS = std::to_string(i + 1);
        pi = "x_p" + S;
        graph.addVertex("(" + x + " and " + pi + ")", "and", "anda" + pi);
        graph.addVertex("(" + y + " and " + pi + ")", "and", "andb" + pi);

        graph.addDoubleEdge(x, y, "andab" + S, false);
        graph.addDoubleEdge(x, pi, "anda" + pi, false);
        graph.addDoubleEdge(y, pi, "andb" + pi, false);

        graph.addVertex("((andab" + S + ")" + " or " + "(anda" + pi + ")" + " or " + "(andb" + pi + "))", "or", "x_p" + NextS);
        graph.addEdge("andab" + S, "x_p" + NextS, false);
        graph.addEdge("anda" + pi, "x_p" + NextS, false);
        graph.addEdge("andb" + pi, "x_p" + NextS, false);
        if (i_overflowOut && i + 1 == i_bits)
        {
            if (act)
            {
                graph.addVertex("(1 and x_p" + NextS + ")", "and", z + "and1_" + NextS);
                graph.addDoubleEdge("1", "x_p" + NextS, z + "and1_" + NextS, false);
            }
            else
            {
                graph.addVertex(z + std::to_string(i_bits), "output");
                graph.addEdge("x_p" + NextS, z + std::to_string(i_bits), false);
            }
        }
        graph.addVertex("not (x_p" + NextS + ")", "not", "np" + NextS);
        graph.addEdge("x_p" + NextS, "np" + NextS, false);

        graph.addVertex("(" + x + " or " + y + " or " + pi + ")", "or", "abpor" + S);
        graph.addEdge(x, "abpor" + S, false);
        graph.addEdge(y, "abpor" + S, false);
        graph.addEdge(pi, "abpor" + S, false);

        graph.addVertex("(abpor" + S + " and np" + NextS + ")", "and", "andnp" + NextS);
        graph.addDoubleEdge("abpor" + S, "np" + NextS, "andnp" + NextS, false);

        graph.addVertex("(" + x + " and " + y + " and " + pi + ")", "and", "abpand" + S);
        graph.addEdge(x, "abpand" + S, false);
        graph.addEdge(y, "abpand" + S, false);
        graph.addEdge(pi, "abpand" + S, false);

        graph.addVertex("(abpand" + S + " or " + "andnp" + NextS + ")", "or", "pS" + S);
        graph.addDoubleEdge("abpand" + S, "andnp" + NextS, "pS" + S, false);
        if (act)
        {
            graph.addVertex("(1 and pS" + S + ")", "and", z + "and1_" + S);
            graph.addDoubleEdge("1", "pS" + S, z + "and1_" + S, false);
        }
        else
        {
            graph.addEdge("pS" + S, z + S, false);
        }

    }

    return graph;
}

OrientedGraph SimpleGenerators::generatorComparison(int i_bits, bool i_compare0, bool i_compare1, bool i_compare2, bool act)
{
    OrientedGraph graph;

    std::string cond = std::string(i_compare0 ? "t" : "f") + (i_compare1 ? "t" : "f") + (i_compare2 ? "t" : "f");
    for (int i = i_bits - 1; i >= 0; i--)
    {
        std::string C = "x_" + std::to_string(i);
        std::string NextC = "x_" + std::to_string(i - 1);
        std::string x = "coma" + cond + C;
        std::string y = "comb" + cond + C;
        if (i == 0)
        {
            NextC = "X";
        }
        graph.addVertex(x, "input");
        graph.addVertex(y, "input");
        graph.addVertex("not (" + y + C + ")", "not", "nb" + C);
        graph.addEdge(y, "nb" + C, false);
        graph.addVertex("not (" + x + C + ")", "not", "na" + C);
        graph.addEdge(x, "na" + C, false);
        if (act)
        {
            graph.addVertex("1", "const");
        }
        if (i_compare0)
        {
            if (!act)
            {
                graph.addVertex("E0_" + C, "output");
            }
            graph.addVertex("(na" + C + " and nb" + C + ")", "and", "nab" + C);
            graph.addVertex("(" + x + C + " and " + y + C + ")", "and", "ab" + C);
            graph.addVertex("(nab" + C + " or ab" + C + ")", "or", "p0_" + NextC);
            graph.addDoubleEdge("na" + C, "nb" + C, "nab" + C, false);
            graph.addDoubleEdge(x, y, "ab" + C, false);
            graph.addDoubleEdge("nab" + C, "ab" + C, "p0_" + NextC, false);

            if (i == i_bits - 1)
            {
                if (act)
                {
                    graph.addVertex("(1 and p0_" + NextC + ")", "and", "E0and1_" + C);
                    graph.addDoubleEdge("1", "p0_" + NextC, "E0and1_" + C, false);
                }
                else
                {
                    graph.addEdge("p0_" + NextC, "E0_" + C, false);
                }
            }
            else
            {
                graph.addVertex("(p0_" + C + " and p0_" + NextC + ")", "and", "pE0_" + C);
                graph.addDoubleEdge("p0_" + C, "p0_" + NextC, "pE0_" + C, false);
                if (act)
                {
                    graph.addVertex("(1 and pE0_" + C + ")", "and", "E0and1_" + C);
                    graph.addDoubleEdge("1", "pE0_" + C, "E0and1_" + C, false);
                }
                else
                {
                    graph.addEdge("pE0_" + C, "E0_" + C, false);
                }
            }
        }
        if (i_compare1)
        {
            if (act)
            {
                graph.addVertex("1", "const");
            }
            else
            {
                graph.addVertex("E1_" + C, "output");
            }

            graph.addVertex("(" + x + C + " and " + "(not (" + y + C + "))", "and", "p1_" + NextC);
            graph.addDoubleEdge(x, "nb" + C, "p1_" + NextC, false);

            if (i == i_bits - 1)
            {
                if (act)
                {
                    graph.addVertex("(1 and p1_" + NextC + ")", "and", "E1and1_" + C);
                    graph.addDoubleEdge("1", "p1_" + NextC, "E1and1_" + C, false);
                }
                else
                {
                    graph.addEdge("p1_" + NextC, "E1_" + C, false);
                }
            }
            else
            {
                graph.addVertex("not (p1_" + C + ")", "not", "np1_" + C);
                graph.addVertex("not (p1_" + NextC + ")", "not", "np1_" + NextC);
                graph.addEdge("p1_" + C, "np1_" + C, false);
                graph.addEdge("p1_" + NextC, "np1_" + NextC, false);
                graph.addVertex("(np1_" + C + " and p1_" + NextC + ")", "and", "P11_" + C);
                graph.addDoubleEdge("np1_" + C, "p1_" + NextC, "P11_" + C, false);
                graph.addVertex("(p1_" + C + " and np1_" + NextC + ")", "and", "P12_" + C);
                graph.addDoubleEdge("p1_" + C, "np1_" + NextC, "P12_" + C, false);
                graph.addVertex("(P11_" + C + " or P12_" + C + ")", "or", "pE1_" + C);
                graph.addDoubleEdge("P11_" + C, "P12_" + C, "pE1_" + C, false);
                if (act)
                {
                    graph.addVertex("(1 and pE1_" + C + ")", "and", "E1and1_" + C);
                    graph.addDoubleEdge("1", "pE1_" + C, "E1and1_" + C, false);
                }
                else
                {
                    graph.addEdge("pE1_" + C, "E1_" + C, false);
                }
            }
        }
        if (i_compare2)
        {
            if (act)
            {
                graph.addVertex("1", "const");
            }
            else
            {
                graph.addVertex("E2_" + C, "output");
            }
            graph.addVertex("(" + y + C + " and " + "(not (" + x + C + "))", "and", "p2_" + NextC);
            graph.addDoubleEdge(y, "na" + C, "p2_" + NextC, false);

            if (i == i_bits - 1)
            {
                if (act)
                {
                    graph.addVertex("(1 and p2_" + NextC + ")", "and", "E2and1_" + C);
                    graph.addDoubleEdge("1", "p2_" + NextC, "E2and1_" + C, false);
                }
                else
                {
                    graph.addEdge("p2_" + NextC, "E2_" + C, false);
                }
            }
            else
            {
                graph.addVertex("not (p2_" + C + ")", "not", "np2_" + C);
                graph.addVertex("not (p2_" + NextC + ")", "not", "np2_" + NextC);
                graph.addEdge("p2_" + C, "np2_" + C, false);
                graph.addEdge("p2_" + NextC, "np2_" + NextC, false);
                graph.addVertex("(np2_" + C + " and p2_" + NextC + ")", "and", "P21_" + C);
                graph.addDoubleEdge("np2_" + C, "p2_" + NextC, "P21_" + C, false);
                graph.addVertex("(p2_" + C + " and np2_" + NextC + ")", "and", "P22_" + C);
                graph.addDoubleEdge("p2_" + C, "np2_" + NextC, "P22_" + C, false);
                graph.addVertex("(P21_" + C + " or P22_" + C + ")", "or", "pE2_" + C);
                graph.addDoubleEdge("P21_" + C, "P22_" + C, "pE2_" + C, false);
                if (act)
                {
                    graph.addVertex("(1 and pE2_" + C + ")", "and", "E2and1_" + C);
                    graph.addDoubleEdge("1", "pE2_" + C, "E2and1_" + C, false);
                }
                else
                {
                    graph.addEdge("pE2_" + C, "E2_" + C, false);
                }
            }
        }
    }
    return graph;
}

OrientedGraph SimpleGenerators::generatorEncoder(int i_bits)
{
    OrientedGraph graph ;
    int k = 0;
    std::vector<std::string> X(pow(2, i_bits));
    for (int t = 0; t <= i_bits; t++)
        if (i_bits - 1 >= pow(2, t))
        {
            k += 1;
        }
    for (int l = 0; l <= i_bits - 1; l++)
    {
        std::string Z = std::to_string(l);
        X[l] = "x" + Z;
        graph.addVertex(X[l], "input");
    }
    if (i_bits > 1)
        for (int p = k - 1; p >= 0; p--)
        {
            std::string L = "";
            std::string P = "";
            std::string M = "";
            std::string K = "";
            std::string S = std::to_string(p);
            graph.addVertex("a" + S, "output");

            for (int i = 0; i <= i_bits - 1; i++)
                for (double t = pow(2, p); t <= pow(2, p + 1) - 1; t++)
                    if (pow(2, p + 1) * i + t <= i_bits - 1)
                    {
                        double r = pow(2, p + 1) * i + t;
                        int R = int(r);
                        K = M + " or " + X[R];
                        L = P + " or " + X[R];
                        //graph.addEdge("x" + R, "a" + S, false);
                        P = L;
                        L = "";
                        M = K;
                        K = "";
                    }
            M = M.erase(0, 3);
            graph.addVertex(M, "or", P);
            for (int i = 0; i <= i_bits - 1; i++)
                for (double t = pow(2, p); t <= pow(2, p + 1) - 1; t++)
                    if (pow(2, p + 1) * i + t <= i_bits - 1)
                    {
                       double r = pow(2, p + 1) * i + t;
                       int R = int(r);
                        graph.addEdge(X[R], P, false);
                    }
            graph.addEdge(P, "a" + S, false);
        }
    else
       std::cout << "Недостаточно входных сигналов\n";

    return graph;
}

OrientedGraph SimpleGenerators::generatorMultiplexer(int i_bits, std::string T)
{
    OrientedGraph graph;
    int k = 0;
    
    for (int t = 0; t <= i_bits; t++) {
        if (i_bits - 1 >= std::pow(2, t)) {
            k = k + 1;
        }
    }

    std::vector<std::string> F(i_bits);
    std::vector<std::string> X(i_bits);
    std::vector<std::string> K(i_bits);
    std::vector<std::string> S(k);
    std::vector<std::string> Z(i_bits);

    graph.addVertex("f" + T, "output");

    if (i_bits > 1) {
        for (int p = 0; p <= k - 1; p++) {
            S[p] = std::to_string(p);
            graph.addVertex("x_a" + T + "_" + S[p], "input");
            graph.addVertex("not (x_a" + T + "_" + S[p] + ")", "not", "na" + T + "_" + S[p]);
            graph.addEdge("x_a" + T + "_" + S[p], "na" + T + "_" + S[p], false);
        }

        for (int i = 0; i <= i_bits - 1; i++) {
            Z[i] = std::to_string(i);
            graph.addVertex("x" + T + "_" + Z[i], "input");
            F[i] = std::bitset<8>(i).to_string(); 
            int len = F[i].length();

            for (int w = 0; w <= k - 1; w++) {
                if (F[i].length() < w + 1) {
                    X[i] = K[i] + " and na" + T + "_" + S[w];
                } else {
                    char u = F[i][len - w - 1];
                    if (u == '1') {
                        X[i] = K[i] + " and x_a" + T + "_" + S[w];
                    } else {
                        X[i] = K[i] + " and na" + T + "_" + S[w];
                    }
                }
                K[i] = X[i];
                X[i] = "";
            }
        }

        for (int i = 0; i <= i_bits - 1; i++) {
            if (!K[i].empty()) {
                K[i].erase(0, 4);
            }
            graph.addVertex(K[i] + " and x" + T + "_" + Z[i], "and", K[i] + " and x" + T + "_" + Z[i]);
            graph.addVertex(K[i] + " and x" + T + "_" + Z[i], "or", "f" + T);
        }

        for (int i = 0; i <= i_bits - 1; i++) {
            int len = F[i].length();
            for (int w = 0; w <= k - 1; w++) {
                if (F[i].length() < w + 1) {
                    graph.addEdge("x_a" + T + "_" + S[w], "na" + T + "_" + S[w], false);
                    graph.addEdge("na" + T + "_" + S[w], K[i] + " and x" + T + "_" + Z[i], false);
                } else {
                    char u = F[i][len - w - 1];
                    if (u == '1') {
                        graph.addEdge("x_a" + T + "_" + S[w], K[i] + " and x" + T + "_" + Z[i], false);
                    } else {
                        graph.addEdge("x_a" + T + "_" + S[w], "na" + T + "_" + S[w], false);
                        graph.addEdge("na" + T + "_" + S[w], K[i] + " and x" + T + "_" + Z[i], false);
                    }
                }
            }
            graph.addEdge("x" + T + "_" + Z[i], K[i] + " and x" + T + "_" + Z[i], false);
            graph.addEdge(K[i] + " and x" + T + "_" + Z[i], "f" + T, false);
        }
    } else {
        std::cout << "Недостаточно выходных сигналов" << std::endl;
    }

    return graph;
}

OrientedGraph SimpleGenerators::generatorParity(int i_bits)
{
    //bits - количество входов
    //T[]- массив для текущего ряда узлов
    //X[]- массив для предыдущего ряда узлов
    //"хi" - входной бит
    //F - функция на выходе
    OrientedGraph graph;
    if (i_bits > 1)
    {
        std::vector<std::string> X(i_bits);
        std::vector<std::string> T(i_bits);
        for (int l = 0; l <= i_bits - 1; l++)
        {
            std::string Z = std::to_string(l);
            graph.addVertex("x" + Z, "input");
            X[l] = "x" + Z;
        }
        graph.addVertex("F", "output");
        int p = i_bits;
        while (p > 1)
        {
            if (p % 2 == 1)
            {
                T[p - 2] = X[p - 2] + " xor " + X[p - 1];
                graph.addVertex(T[p - 2], "xor", T[p - 2]);
                graph.addDoubleEdge(X[p - 2], X[p - 1], T[p - 2], false);
                X[p - 2] = T[p - 2];
                T[p - 2] = "";
                p = p - 1;
            }
            p = p / 2;
            for (int i = 0; i <= p - 1; i++)
                T[i] = X[2 * i] + " xor " + X[2 * i + 1];
            for (int i = 0; i <= p - 1; i++)
            {
                graph.addVertex(T[i], "xor", T[i]);
                graph.addDoubleEdge(X[2 * i], X[2 * i + 1], T[i], false);
            }
            for (int i = 0; i <= p - 1; i++)
            {
                X[i] = T[i];
                T[i] = "";
            }
        }
        graph.addEdge(X[0], "F", false);
    } else
        std::cout << "Недостаточно входных сигналов" << std::endl;
    return graph;
}

OrientedGraph SimpleGenerators::generatorMultiplier(int i_bits, bool act)
{
    //a - бит первого множителя
    //b - бит второго множителя
    //с - результат логического и
    //s - результат суммы
    //p - перенос
    //m - бит полученного умножения
    OrientedGraph graph;
    int n = 1;

    for (int ib = 1; ib <= i_bits; ib++){

        std::string IB = std::to_string(ib); //IB - index b
        std::string IBP = std::to_string(ib - 1); //IBP - index b past
        std::string IBN = std::to_string(ib + 1); //IBN - index b next

        if (act)
        {
            graph.addVertex("1", "const");
        }

        graph.addVertex("xb" + IB, "input");
        for (int ia = 1; ia <= i_bits; ia++){
            std::string IA = std::to_string(ia); //IA - index a
            std::string IAN = std::to_string(ia + 1); //IAN - index a next

            graph.addVertex("xa" + IA, "input");

            graph.addVertex("(xa" + IA + " and xb" + IB + ")", "and", "c" + IA + IB);
            graph.addDoubleEdge("xa" + IA, "xb" + IB, "c" + IA + IB, false);

            if (ib == 1){
                if (ia == 1){
                    std::string N = std::to_string(n);

                    if (act)
                    {
                        graph.addVertex("(1 and c" + IA + IB + ")", "and", "aluR" + N); //aluR - alu Result
                        graph.addDoubleEdge("1", "c" + IA + IB, "aluR" + N, false);
                    }

                    else
                    {
                        graph.addVertex("m" + N, "output");
                        graph.addEdge("c" + IA + IB, "m" + N, false);
                    }
                    n += 1;
                }
            }
            if (ib > 1){
                std::string nowAB = "c" + IA + IB;
                std::string ABsum;
                if (ib == 2)
                    ABsum = "c" + IAN + IBP; //второй разряд, вход в сумматор от операции И
                if (ib > 2)
                    ABsum = "sum" + IAN + IBP;//следующие разряды, вход в сумматор от результата др. сумматора
                    if (ia == i_bits){
                        ABsum = "pNext" + IA + IB; //для левых боковых сумматоров
                    }

                std::string nSum;
                if (ia < i_bits)
                    nSum = IAN + IB; //nSum - next Summator, по разряду a
                if (ia == i_bits)
                    nSum = IA + IBN; //по разряду b

                if (ia == 1){
                    std::string N = std::to_string(n);
                    graph.addVertex("(" + nowAB + " and " + ABsum + ")", "and", "pSum" + nSum);
                    graph.addDoubleEdge(nowAB, ABsum, "pSum" + nSum, false);
                    graph.addVertex("(" + nowAB + " xor " + ABsum + ")", "xor", "sum" + IA + IB);
                    graph.addDoubleEdge(nowAB, ABsum, "sum" + IA + IB, false);

                    if (act){
                        graph.addVertex("(1 and sum" + IA + IB + ")", "and", "aluR" + N);
                        graph.addDoubleEdge("1", "sum" + IA + IB, "aluR" + N, false);
                    }
                    else{
                        graph.addVertex("m" + N, "output");
                        graph.addEdge("sum" + IA + IB, "m" + N, false);
                    }
                    n+=1;
                }
                else if(ib == 2 && ia == i_bits){
                    graph.addVertex("(" + nowAB + " and pSum" + IA + IB + ")", "and", "pNext" + nSum);
                    graph.addDoubleEdge(nowAB, "pSum" + IA + IB, "pNext" + nSum, false);
                    graph.addVertex("(" + nowAB + " xor pSum" + IA + IB + ")", "xor", "sum" + IA + IB);
                    graph.addDoubleEdge(nowAB, "pSum" + IA + IB, "sum" + IA + IB, false);

                    if (i_bits == 2){
                        std::string N = std::to_string(n);
                        if (act){
                            graph.addVertex("(1 and sum" + IA + IB + ")", "and", "aluR" + N);
                            graph.addDoubleEdge("1", "sum" + IA + IB, "aluR" + N, false);

                            n+=1;
                            std::string N = std::to_string(n);

                            graph.addVertex("(1 and pNext" + nSum + ")", "and", "aluR" + N);
                            graph.addDoubleEdge("1", "pNext" + nSum, "aluR" + N, false);
                        }
                        else{
                            graph.addVertex("m" + N, "output");
                            graph.addEdge("sum" + IA + IB, "m" + N, false);

                            n+=1;
                            std::string N = std::to_string(n);

                            graph.addVertex("m" + N, "output");
                            graph.addEdge("pNext" + nSum, "m" + N, false);
                        }
                        n+=1;
                    }
                }
                else{
                    std::string x = nowAB;
                    std::string y = ABsum;
                    std::string p; //создание переноса нынешнего сумматора в следующий
                    std::string pi = "pSum" + IA + IB; //перенос из прошлого сумматора

                    std::string S = IA + IB;

                    graph.addVertex("(" + x + " and " + y + ")", "and", "andab" + S);
                    graph.addVertex("(" + x + " and " + pi + ")", "and", "anda" + pi);
                    graph.addVertex("(" + y + " and " + pi + ")", "and", "andb" + pi);

                    graph.addDoubleEdge(x, y, "andab" + S, false);
                    graph.addDoubleEdge(x, pi, "anda" + pi, false);
                    graph.addDoubleEdge(y, pi, "andb" + pi, false);

                    if (ia < i_bits){
                        p = "pSum" + nSum; //для соседнего сумматора
                    }
                    if (ia == i_bits){
                        p = "pNext" + nSum; //для левых боковых сумматоров
                    }
                    graph.addVertex("((andab" + S + ") or (anda" + pi + ") or (andb" + pi + "))", "or", p);
                    graph.addEdge("andab" + S, p, false);
                    graph.addEdge("anda" + pi, p, false);
                    graph.addEdge("andb" + pi, p, false);

                    graph.addVertex("not (" + p + ")", "not", "n" + p);
                    graph.addEdge(p, "n" + p, false);

                    graph.addVertex("(" + x + " or " + y + " or " + pi + ")", "or", "abpor" + S);
                    graph.addEdge(x, "abpor" + S, false);
                    graph.addEdge(y, "abpor" + S, false);
                    graph.addEdge(pi, "abpor" + S, false);

                    graph.addVertex("(abpor" + S + " and n" + p + ")", "and", "andnp" + nSum);
                    graph.addDoubleEdge("abpor" + S, "n" + p, "andnp" + nSum, false);

                    graph.addVertex("(" + x + " and " + y + " and " + pi + ")", "and", "abpand" + S);
                    graph.addEdge(x, "abpand" + S, false);
                    graph.addEdge(y, "abpand" + S, false);
                    graph.addEdge(pi, "abpand" + S, false);

                    graph.addVertex("(abpand" + S + " or " + "andnp" + nSum + ")", "or", "sum" + IA + IB);
                    graph.addDoubleEdge("abpand" + S, "andnp" + nSum, "sum" + IA + IB, false);

                    if (ib == i_bits){
                        std::string N = std::to_string(n);
                        if (act){
                            graph.addVertex("(1 and sum" + IA + IB + ")", "and", "aluR" + N);
                            graph.addDoubleEdge("1", "sum" + IA + IB, "aluR" + N, false);
                        }
                        else{
                            graph.addVertex("m" + N, "output");
                            graph.addEdge("sum" + IA + IB, "m" + N, false);
                        }
                        n+=1;
                        if (ia == i_bits){
                            N = std::to_string(n);
                            if (act){
                                graph.addVertex("(1 and " + p + ")", "and", "aluR" + N);
                                graph.addDoubleEdge("1", p, "aluR" + N, false);
                            }
                            else{
                                graph.addVertex("m" + N, "output");
                                graph.addEdge(p, "m" + N, false);
                            }
                            n+=1;
                        }
                    }
                }
            }
        }
    }
    return graph;
}

OrientedGraph SimpleGenerators::generatorDecoder(int i_bits)
{
    //bits - количество входов
    //T[]- массив для текущего ряда узлов
    //X[]- массив для предыдущего ряда узлов
    //"хi" - входной бит
    //F - функция на выходе
    OrientedGraph graph;
    if (i_bits > 1)
    {
        std::vector<std::string> T(pow(2, i_bits));
        std::vector<std::string> X(pow(2, i_bits));
        for (int i = 0; i < i_bits; i++)
        {
            std::string Z = std::to_string(i);
            graph.addVertex("x" + Z, "input");
            graph.addVertex("not (x"+ Z + ")", "not", "not (x"+ Z + ")");
            X[i] = "x" + Z;
            X[i+i_bits] = "not (x"+ Z + ")";
            graph.addEdge(X[i], X[i+i_bits], false);
        }
        for (int i = 0; i < pow(2, i_bits); i++)
        {
            std::string Z = std::to_string(i);
            graph.addVertex("f" + Z, "output");
        }
        int p = i_bits;
        for (int i = 0; i < pow(2, i_bits); i++)
        {
            std::bitset<sizeof(int)*8> bs(i);
            std::string bit_string = bs.to_string();
            std::string res = bit_string.substr(bit_string.length()-i_bits);
            for (int j = 0; j < res.length(); j++)
            {
                if (res[j] == '1')
                {
                    T[i] += std::string("(" + X[j] + ")");
                }
                else
                {
                    T[i] += std::string("(" + X[j+i_bits] + ")");
                }
                if (j == res.length()-1)
                {
                    T[i] += std::string("+ and +");
                }
            }
            graph.addVertex(T[i], "and", T[i]);
            for (int j = 0; j < res.length(); j++)
            {
                if (res[j] == '1')
                {
                    graph.addEdge(X[j], T[i], false);
                }
                else
                {
                    graph.addEdge(X[j+i_bits], T[i], false);
                }
            }
        }
        for (int i = 0; i < pow(2, i_bits); i++)
        {
            graph.addEdge(T[i], "f" + std::to_string(i), false);
        }
    }
    else
    {
        graph.addVertex("x", "input");
        graph.addVertex("not x", "not", "not x");
        graph.addEdge("x", "not x", false);
        for (int i = 0; i < 2; i++)
        {
            graph.addVertex("f"+ std::to_string(i), "output");
            if (i == 0)
            {
                graph.addEdge("not x", "f"+ std::to_string(i), false);
            }
            else
            {
                graph.addEdge("x", "f"+ std::to_string(i), false);
            }
        }
    }
    return graph;
}

OrientedGraph SimpleGenerators::generatorDemultiplexer(int i_bits)
{
    //i_bits - количество выходных сигналов, то есть количество х
    //f - значение функции на входе
    //k - количество адресных входов, то есть количество а
    //F[] - массив для хранения индексов х в двоичном формате в строковом виде
    //X[] - массив для создания и хранения строк, аналогичных "a0 and a1 and not a2 and f"
    //K[] - вспомогательный массив для создания строк, аналогичных "a0 and a1 and not a2 and f"
    //S[] - массив для стринговых индексов а 
    //Z[] - массив для стринговых индексов х 
    //len - количество символов в конкретной строке F[i], то есть двоичая разрядность числа хi
    //u - переменная для чтения одного символа из строки F[i]
    
    OrientedGraph graph;
    graph.addVertex("f","input");

    int k = 0;
    for (int t = 0; t <= i_bits; t++) {
        if (i_bits - 1 >= std::pow(2, t))
            k++;
    }

    std::vector<std::string> F(i_bits);
    std::vector<std::string> X(i_bits);
    std::vector<std::string> K(i_bits);
    std::vector<std::string> S(k);
    std::vector<std::string> Z(i_bits);

    if (i_bits > 1) {
        for (int p = 0; p <= k - 1; p++) {
            S[p] = std::to_string(p);
            graph.addVertex("x_a" + S[p], "input");
            graph.addVertex("not x_a" + S[p], "not", "not x_a" + S[p]);
        }

        for (int i = 0; i <= i_bits - 1; i++) {
            Z[i] = std::to_string(i);
            graph.addVertex("x" + Z[i], "output");
            std::string tempString = std::bitset<8>(i).to_string();
            while (tempString[0] == '0')
                tempString.erase(0, 1);
            F[i] = tempString;
            int len = F[i].size();

            for (int w = 0; w <= k - 1; w++) {
                if (F[i].size() < w + 1) 
                    X[i] = K[i] + " and not x_a" + S[w];
                else {
                    char u = F[i][len - w - 1];
                    if (u == '1')
                        X[i] = K[i] + " and x_a" + S[w];
                    else
                        X[i] = K[i] + " and not x_a" + S[w];
                }
                K[i] = X[i];
                X[i] = "";
            }
            X[i] = K[i] + " and f";
        }

        for (int i = 0; i <= i_bits - 1; i++) {
            if (!X[i].empty())
                X[i].erase(0, 5);
            graph.addVertex(X[i], "and", X[i]);
            graph.addEdge("f", X[i], false);
            graph.addEdge(X[i],"x" + Z[i],false);
        }

        for (int i = 0; i <= i_bits - 1; i++) {
            int len = F[i].size();
            for (int w = 0; w <= k - 1; w++) {
                if (F[i].size() < w + 1) {
                    graph.addEdge("x_a" + S[w], "not x_a" + S[w], false);
                    graph.addEdge("not x_a" + S[w], X[i], false);
                }
                else {
                    char u = F[i][len - w - 1];
                    if (u == '1') {
                        graph.addEdge("x_a" + S[w], X[i], false);
                    }
                    else {
                        graph.addEdge("x_a" + S[w], "not x_a" + S[w], false);
                        graph.addEdge("not x_a" + S[w], X[i], false);
                    }
                }
            }
        }
    }
    else
        std::cout << "Недостаточно входных сигналов" << std::endl;
    return graph;
}

OrientedGraph SimpleGenerators::ALU(int i_bits, int i_outbits, bool ALL, bool SUM, bool SUB, bool NSUM, bool NSUB, bool MULT,
                                    bool COM, bool AND, bool NAND, bool OR, bool NOR, bool XOR, bool XNOR, bool CNF){
    OrientedGraph graph;
    if (ALL){
        SUM = true; SUB = true; NSUM = true; NSUB = true; MULT = true; COM = true; AND = true; OR = true; NOR = true; XOR = true; XNOR = true; CNF = true;
    }

    if (SUM){
        graph.Extend(generatorSummator(i_bits, false, false, false, true));
        graph.Extend(generatorSummator(i_bits, false, true, false, true));
        graph.Extend(generatorSummator(i_bits, true, true, false, true));
        graph.Extend(generatorSummator(i_bits, true, false, false, true));
    }

    if (SUB){
        /*
        graph.Extend(generatorSubtractor(i_bits, false, false, false, true));
        graph.Extend(generatorSubtractor(i_bits, false, true, false, true));
        graph.Extend(generatorSubtractor(i_bits, true, false, false, true));
        graph.Extend(generatorSubtractor(i_bits, true, true, false, true));
         */
    }

    if (NSUM)
    {
        graph.Extend(generatorSummator(i_bits, false, false, true, true));
        graph.Extend(generatorSummator(i_bits, false, true, true, true));
        graph.Extend(generatorSummator(i_bits, true, true, true, true));
        graph.Extend(generatorSummator(i_bits, true, false, true, true));

    }

    if (NSUB)
    {
        /*
        graph.Extend(generatorSubtractor(i_bits, false, false, true, true));
        graph.Extend(generatorSubtractor(i_bits, false, true, true, true));
        graph.Extend(generatorSubtractor(i_bits, true, false, true, true));
        graph.Extend(generatorSubtractor(i_bits, true, true, true, true));
         */
    }

    if (MULT)
    {
        graph.Extend(generatorMultiplier(i_bits, true));
    }
    if (COM)
    {
        graph.Extend(generatorComparison(i_bits, true, false, false, true));
        graph.Extend(generatorComparison(i_bits, false, true, false, true));
        graph.Extend(generatorComparison(i_bits, false, false, true, true));
    }

    if (CNF)
    {
        TruthTable tt(i_bits, i_outbits, {});
        tt.generateRandom({i_bits, i_outbits});

        SimpleGenerators tftt;

        std::vector<std::pair<std::string, std::vector<std::string>>> circs;
        circs.push_back({"CNFT", tftt.cnfFromTruthTable(tt, true)});
        circs.push_back({"CNFF", tftt.cnfFromTruthTable(tt, false)});

        //не может понять, что такое Parser
        /*
        for (const auto& [name, expr] : circs)
        {
            Parser pCNFT(expr);
            pCNFT.parseAll();
            graph.Extend(pCNFT.getGraph());
        }
         */
    }

    for (int i = 0; i < i_bits; i++)
    {
        std::string A = std::to_string(i);
        graph.addVertex("xi" + A, "input");
        graph.addVertex("xj" + A, "input");

        if (SUM)
        {
            graph.Substitute("xi" + A, "x_sumafff" + A, false);
            graph.Substitute("xj" + A, "x_sumbfff" + A, false);

            graph.Substitute("xi" + A, "x_sumaftf" + A, false);
            graph.Substitute("xj" + A, "x_sumbftf" + A, false);

            graph.Substitute("xi" + A, "x_sumattf" + A, false);
            graph.Substitute("xj" + A, "x_sumbttf" + A, false);

            graph.Substitute("xi" + A, "x_sumatff" + A, false);
            graph.Substitute("xj" + A, "x_sumbtff" + A, false);
        }

        if (SUB)
        {
            /*
            graph.Substitute("xi" + A, "x_subafff" + A, false);
            graph.Substitute("xj" + A, "x_subbfff" + A, false);

            graph.Substitute("xi" + A, "x_subaftf" + A, false);
            graph.Substitute("xj" + A, "x_subbftf" + A, false);

            graph.Substitute("xi" + A, "x_subatff" + A, false);
            graph.Substitute("xj" + A, "x_subbtff" + A, false);

            graph.Substitute("xi" + A, "x_subattf" + A, false);
            graph.Substitute("xj" + A, "x_subbttf" + A, false);
             */
        }
        if (NSUM)
        {
            graph.Substitute("xi" + A, "x_sumafft" + A, false);
            graph.Substitute("xj" + A, "x_sumbfft" + A, false);

            graph.Substitute("xi" + A, "x_sumaftt" + A, false);
            graph.Substitute("xj" + A, "x_sumbftt" + A, false);

            graph.Substitute("xi" + A, "x_sumattt" + A, false);
            graph.Substitute("xj" + A, "x_sumbttt" + A, false);

            graph.Substitute("xi" + A, "x_sumatft" + A, false);
            graph.Substitute("xj" + A, "x_sumbtft" + A, false);
        }
        if (NSUB)
        {
            /*
            graph.Substitute("xi" + A, "x_subafft" + A, false);
            graph.Substitute("xj" + A, "x_subbfft" + A, false);

            graph.Substitute("xi" + A, "x_subaftt" + A, false);
            graph.Substitute("xj" + A, "x_subbftt" + A, false);

            graph.Substitute("xi" + A, "x_subatft" + A, false);
            graph.Substitute("xj" + A, "x_subbtft" + A, false);

            graph.Substitute("xi" + A, "x_subattt" + A, false);
            graph.Substitute("xj" + A, "x_subbttt" + A, false);
             */
        }
        if (MULT)
        {
            graph.Substitute("xi" + A, "xa" + A, false);
            graph.Substitute("xj" + A, "xb" + A, false);
        }
        if (COM)
        {
            graph.Substitute("xi" + A, "comatffx_" + A, false);
            graph.Substitute("xj" + A, "combtffx_" + A, false);

            graph.Substitute("xi" + A, "comaftfx_" + A, false);
            graph.Substitute("xj" + A, "combftfx_" + A, false);

            graph.Substitute("xi" + A, "comafftx_" + A, false);
            graph.Substitute("xj" + A, "combfftx_" + A, false);
        }
        if (AND)
        {
            graph.addVertex("(xi" + A + " and xj" + A + ")", "and", "ij1_" + A);
            graph.addDoubleEdge("xi" + A, "xj" + A, "ij1_" + A, false);
        }
        if (NAND)
        {
            graph.addVertex("(xi" + A + " nand xj" + A + ")", "nand", "ij2_" + A);
            graph.addDoubleEdge("xi" + A, "xj" + A, "ij2_" + A, false);

        }
        if (OR)
        {
            graph.addVertex("(xi" + A + " or xj" + A + ")", "or", "ij3_" + A);
            graph.addDoubleEdge("xi" + A, "xj" + A, "ij3_" + A, false);
        }
        if (XOR)
        {
            graph.addVertex("(xi" + A + " xor xj" + A + ")", "xor", "ij5_" + A);
            graph.addDoubleEdge("xi" + A, "xj" + A, "ij5_" + A, false);

        }
        if (NOR)
        {
            graph.addVertex("(xi" + A + " nor xj" + A + ")", "nor", "ij4_" + A);
            graph.addDoubleEdge("xi" + A, "xj" + A, "ij4_" + A, false);
        }
        if (XNOR)
        {
            graph.addVertex("(xi" + A + " xnor xj" + A + ")", "xnor", "ij6_" + A);
            graph.addDoubleEdge("xi" + A, "xj" + A, "ij6_" + A, false);
        }
        if (CNF)
        {
            /*
            graph.Substitute("xi" + A, "xT" + A, false);
            graph.Substitute("xi" + A, "xF" + A, false);
             */
        }
    }
    return graph;
}

OrientedGraph SimpleGenerators::generatorALU(int i_bits, int i_outbits, bool SUM, bool SUB, bool NSUM, bool NSUB, bool MULT,
                                             bool COM, bool AND, bool NAND, bool OR, bool NOR, bool XOR, bool XNOR, bool CNF, bool ALL){
    OrientedGraph graph;
    int x = 0;
    if (ALL)
    {
        SUM = true; SUB = true; NSUM = true; NSUB = true; MULT = true; COM = true; AND = true; OR = true; XOR = true; XNOR = true; CNF = true;
    }

    graph.Extend(ALU(i_bits, i_outbits, ALL, SUM, SUB, NSUM, NSUB, MULT, COM, AND, NAND, OR, NOR, XOR, XNOR, CNF));

    x = x + (SUM ? 4 : 0) + (SUB ? 4 : 0) + (NSUM ? 4 : 0) + (NSUB ? 4 : 0) + (MULT ? 1 : 0) + (COM ? 3 : 0) + (AND ? 1 : 0) + (NAND ? 1 : 0) + (OR ? 1 : 0) + (NOR ? 1 : 0) + (XOR ? 1 : 0) + (XNOR ? 1 : 0) + (CNF ? 2 : 0);

    int size = i_bits;

    size = MULT ? i_bits * 2 : (SUM || NSUM || SUB || NSUB ? i_bits + 1 : i_bits);
    if (CNF)
    {
        graph.addVertex("0", "const");
        if (MULT)
        {
            size = (i_outbits < i_bits * 2 ? i_bits * 2 : i_outbits);
        }
        else if (SUM || NSUM || SUB || NSUB)
        {
            size = (i_outbits < i_bits + 1 ? i_bits + 1 : i_outbits);
        }
        else if (i_outbits > i_bits)
        {
            size = i_outbits;
        }
    }
    else if (MULT)
    {
        size = i_bits * 2;
        graph.addVertex("0", "const");
    }
    else if (SUM || NSUM || SUB || NSUB)
    {
        size = i_bits + 1;
        graph.addVertex("0", "const");
    }
    for (int j = 0; j < size; j++)
    {
        std::string T = std::to_string(j);
        graph.Extend(generatorMultiplexer(x, T));
        int c = 0;
        if (SUM)
        {
            for (int i = c; i < 4; i++)
            {
                std::string T1 = std::to_string(c); std::string T01 = std::to_string(i);
                c += 1;
                if (MULT ? (j == i_bits && (i == 0 || i == 2)) || j > i_bits : j == i_bits && (i == 0 || i == 2))
                {
                    graph.Substitute("0", "x" + T + "_" + T1, false);
                }
                else
                {
                    graph.Substitute("s" + T01 + "and1_" + T, "x" + T + "_" + T1, false);
                }

            }

        }
        if (NSUM)
        {
            for (int i = c; i < 4; i++)
            {
                std::string T1 = std::to_string(c); std::string T01 = std::to_string(i);
                c += 1;
                if (MULT ? (j == i_bits && (i == 0 || i == 2)) || j > i_bits : j == i_bits && (i == 0 || i == 2))
                {
                    graph.Substitute("0", "x" + T + "_" + T1, false);
                }
                else
                {
                    graph.Substitute("ns" + T01 + "and1_" + T, "x" + T + "_" + T1, false);
                }
            }


        }
        if (SUB)
        {
            for (int i = c; i < 4; i++)
            {
                std::string T1 = std::to_string(c); std::string T01 = std::to_string(i);
                c += 1;
                if (MULT ? (j == i_bits && (i == 0 || i == 2)) || j > i_bits : j == i_bits && (i == 0 || i == 2))
                {
                    graph.Substitute("0", "x" + T + "_" + T1, false);
                }
                else
                {
                    graph.Substitute("d" + T01 + "and1_" + T, "x" + T + "_" + T1, false);
                }
            }
        }
        if (NSUB)
        {
            for (int i = c; i < 4; i++)
            {
                std::string T1 = std::to_string(c); std::string T01 = std::to_string(i);
                c += 1;

                if (MULT ? (j == i_bits && (i == 0 || i == 2)) || j > i_bits : j == i_bits && (i == 0 || i == 2))
                {
                    graph.Substitute("0", "x" + T + "_" + T1, false);
                }
                else
                {
                    graph.Substitute("nd" + T01 + "and1_" + T, "x" + T + "_" + T1, false);
                }
            }
        }
        if (MULT)
        {

            std::string T1 = std::to_string(c);
            c += 1;
            if (j >= i_bits * 2)
            {
                graph.Substitute("0", "x" + T + "_" + T1, false);
            }
            else
            {
                graph.Substitute("mand1_" + T, "x" + T + "_" + T1, false);
            }

        }
        if (COM)
        {
            for (int i = c; i < 3; i++)
            {
                std::string T1 = std::to_string(c); std::string T01 = std::to_string(i);
                c += 1;
                if (j >= i_bits)
                {
                    graph.Substitute("0", "x" + T + "_" + T1, false);
                }
                else
                {
                    graph.Substitute("E" + T01 + "and1_" + T, "x" + T + "_" + T1, false);
                }

            }
        }
        if (AND)
        {

            std::string T1 = std::to_string(c);
            if (j >= i_bits)
            {
                graph.Substitute("0", "x" + T + "_" + T1, false);
            }
            else
            {
                graph.Substitute("ij1_" + T, "x" + T + "_" + T1, false);
            }

            c += 1;

        }
        if (NAND)
        {
            std::string T1 = std::to_string(c);
            if (j >= i_bits)
            {
                graph.Substitute("0", "x" + T + "_" + T1, false);
            }
            else
            {
                graph.Substitute("ij2_" + T, "x" + T + "_" + T1, false);
            }

            c += 1;

        }
        if (OR)
        {

            std::string T1 = std::to_string(c);
            if (j >= i_bits)
            {
                graph.Substitute("0", "x" + T + "_" + T1, false);
            }
            else
            {
                graph.Substitute("ij3_" + T, "x" + T + "_" + T1, false);
            }

            c += 1;

        }
        if (XOR)
        {
            std::string T1 = std::to_string(c);
            if (j >= i_bits)
            {
                graph.Substitute("0", "x" + T + "_" + T1, false);
            }
            else
            {
                graph.Substitute("ij5_" + T, "x" + T + "_" + T1, false);
            }

            c += 1;
        }
        if (NOR)
        {
            std::string T1 = std::to_string(c);
            if (j >= i_bits)
            {
                graph.Substitute("0", "x" + T + "_" + T1, false);
            }
            else
            {
                graph.Substitute("ij4_" + T, "x" + T + "_" + T1, false);
            }

            c += 1;
        }
        if (XNOR)
        {
            std::string T1 = std::to_string(c);
            if (j >= i_bits)
            {
                graph.Substitute("0", "x" + T + "_" + T1, false);
            }
            else
            {
                graph.Substitute("ij6_" + T, "x" + T + "_" + T1, false);
            }

            c += 1;
        }
        if (CNF)
        {
            std::string T1 = std::to_string(c);
            if (j >= i_outbits)
            {
                graph.Substitute("0", "x" + T + "_" + T1, false);
                c += 1;
                T1 = std::to_string(c);
                graph.Substitute("0", "x" + T + "_" + T1, false);
            }
            else
            {
                graph.Substitute("fT" + T, "x" + T + "_" + T1, false);
                c += 1;
                T1 = std::to_string(c);
                graph.Substitute("fF" + T, "x" + T + "_" + T1, false);
            }

        }
    }
    return graph;
}