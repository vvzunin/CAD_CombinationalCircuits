#pragma once

#include <string>
#include <memory>

#include <graph/OrientedGraph.h>

/// class Parser
/// @param d_logExpressions It can represent a set of logical expressions describing the logic of combinational schemes. These expressions can contain combinations of logical operations and variables. for example, A AND B, NOT, C XOR(A, Y)*
/// @param d_graph An object of the Oriented Graph class, which is an oriented graph used to represent relationships between logical expressions.
/// @param d_settings A pointer to an object of the Settings class, which is a singleton and is used to store settings related to the "Parser"
/// 
/// </summary>

class Parser
{
public:
    Parser(const std::string &i_logExpression);
    Parser(const std::vector<std::string> &i_logExpressions);
    OrientedGraph getGraph() const;
    std::pair<int, std::vector<std::string>> splitLogicExpression(std::string i_expr);
    bool parse(const std::string &i_expr);
    bool parseAll();

private:
    std::vector<std::string> d_logExpressions;

    OrientedGraph d_graph;
    std::shared_ptr<Settings> d_settings = Settings::getInstance("Parser");

    std::pair<bool, std::vector<std::pair<int, int>>> createBrackets(const std::string &i_expr);
    bool inBrackets(const std::vector<std::pair<int, int>> &i_brackets, int i_position) const;
    std::string deleteExtraSpaces(std::string i_s);
};
