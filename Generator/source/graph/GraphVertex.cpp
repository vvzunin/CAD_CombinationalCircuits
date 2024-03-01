#include <string>

#include "GraphVertex.h"
// TODO why here are two variables
int GraphVertex::d_count = 0;
GraphVertex::GraphVertex(const std::string& i_expr,
                         const std::string& i_operation,
                         bool i_value,
                         const std::string& i_wireName):
                         d_logicExpression(i_expr),
                         d_operation(i_operation),
                         d_value(i_value)
{
  static int d_count = 0;
  if (d_operation == "input" || d_operation == "output" || d_operation == "const")
    d_wireName = i_expr;
  else
  {
      if (i_wireName == "")
      {
          std::string var = "d";
          d_wireName += var + std::to_string(d_count++);
      }
      else
      {
          d_wireName = i_wireName;
      }
  }
}

void GraphVertex::setWireName(std::string i_wireName)
{
    d_wireName = i_wireName;
}

bool GraphVertex::operator== (const GraphVertex& r) const
{
  return std::tie(d_level, d_logicExpression, d_operation)
  == std::tie(r.d_level, r.d_logicExpression, r.d_operation);
}









int GraphVertex::getLevel() const
{
  return d_level;
}

void GraphVertex::setLevel(int i_level)
{
  d_level = i_level;
}

bool GraphVertex::getValue() const
{
    if(!wrongVertex)
    { 
        return d_value;
    }
    else
    {
        return !d_value;
    }
}

void GraphVertex::setValue(bool i_value)
{
  d_value = i_value;
}

void GraphVertex::setLogicExpression(const std::string& i_logicExpression)
{
  d_logicExpression = i_logicExpression;
}

std::string GraphVertex::getLogicExpression() const
{
  return d_logicExpression;
}

std::string GraphVertex::getOperation() const
{
  return d_operation;
}

void GraphVertex::setOperation(const std::string& i_operation)
{
  d_operation = i_operation;
}

std::string GraphVertex::getWireName() const
{
  return d_wireName;
}
