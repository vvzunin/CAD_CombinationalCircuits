#pragma once

#include <string>
#include <memory>

#include <settings/Settings.h>

/// class GraphVertex
/// @param d_logicExpression Stores a logical expression associated with a graph vertex
/// @param d_operation Stores the operation associated with the vertex of the graph. This operation can be any operation defined for a given vertex, for example, "AND", "OR", "NOT", "XOR", etc.
/// @param d_level The level of the vertex in the graph
/// @param d_value The value of the vertex. This Boolean value represents the current state of the vertex, which can be true or false depending on the execution of the operation or the calculation of the Boolean expression.
/// @param d_wireName This name can be used to identify the wire or connection associated with a given vertex in the graph*
/// @param d_count This static field is used to count the number of Graph Vertex class objects created
/// @param d_settings A pointer to the settings object for the vertex of the graph
/// 
/// 
/// */

class GraphVertex {
public:
  GraphVertex(const std::string& i_expr,
              const std::string& i_operation,
              bool i_value = false,
              const std::string& i_wireName = ""
             );
  int getLevel() const;
  void setLevel(int i_level);
  bool getValue() const;
  void setWireName(std::string i_wireName);
  void setValue(bool i_value);
  void setLogicExpression(const std::string& i_logicExpression);
  std::string getLogicExpression() const;
  std::string getOperation() const;
  void setOperation(const std::string& i_operation);
  std::string getWireName() const;
  bool wrongVertex = false;
private:
  std::string d_logicExpression = "";
  std::string d_operation = "";
  int d_level = 0;
  bool d_value = false;
  std::string d_wireName = "";
  static int d_count;
  
  std::shared_ptr<Settings> d_settings = Settings::getInstance("GraphVertex");
};
