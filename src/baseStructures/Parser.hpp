
#pragma once

#include <memory>
#include <stack>
#include <string>
#include <vector>

#include "graph/OrientedGraph.hpp"
using GatesInfo = std::map<Gates, std::vector<int>>;

class Parser {
public:
  // TODO: Переделать парсер под новую реализацию.
  Parser(const std::string& i_logExpression);
  Parser(const std::vector<std::string>& i_logExpressions);
  GraphPtr                                     getGraph() const;
  std::pair<int32_t, std::vector<std::string>> splitLogicExpression(
      std::string i_expr
  );
  bool      parse(const std::string& i_expr);

  VertexPtr multipleVerteciesToOne(
      std::vector<VertexPtr> curLayout,
      Gates                  operation,
      GraphPtr               graph
  );
  VertexPtr parseToVertex(const std::string& i_expr);
  bool      parseAll();

  void      setGatesInputsInfo(const GatesInfo& i_info);

private:
  std::vector<std::string>  d_logExpressions;

  GraphPtr                  d_graph;
  std::shared_ptr<Settings> d_settings = Settings::getInstance("Parser");

  std::pair<bool, std::vector<std::pair<int32_t, int32_t>>> createBrackets(
      const std::string& i_expr
  );
  bool inBrackets(
      const std::vector<std::pair<int32_t, int32_t>>& i_brackets,
      int32_t                                         i_position
  ) const;
  std::string                      deleteExtraSpaces(std::string i_s);
  std::map<std::string, VertexPtr> inputsByNames;
  std::map<std::string, VertexPtr> notInputsByNames;

  std::map<std::string, std::stack<VertexPtr>> operationByCreatedVertex;
  std::map<std::string, std::stack<VertexPtr>> inputsByCreatedVertex;
  std::stack<std::string>                      doneOperations;
  GatesInfo                                    d_gatesInputsInfo;
};
