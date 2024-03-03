#include <string>

#include "GraphVertexBase.h"

int GraphVertexBase::d_count = 0;

GraphVertexBase::GraphVertexBase(OrientedGraph* const i_baseGraph, const VertexTypes i_type, const std::string i_name) {
  d_baseGraph = i_baseGraph;
  d_type = i_type;
  d_name = i_name;
}

GraphVertexBase::GraphVertexBase( OrientedGraph* const i_baseGraph, 
                                  VertexTypes i_type, 
                                  const std::string i_name,
                                  int i_inputs,
                                  int i_outputs) : d_type{i_type} {
  d_baseGraph = i_baseGraph;
  d_name = this->getTypeName() + "_" + std::to_string(d_count++);
  d_inConnections.resize(i_inputs >= 0 ? i_inputs : 0);
  d_outConnections.resize(i_outputs >= 0 ? i_outputs : 0);
  d_value = 'x';
  d_level = 0;
}

GraphVertexBase::~GraphVertexBase() {
  
}

VertexTypes GraphVertexBase::getType() const {
  return d_type;
}

std::string GraphVertexBase::getTypeName() const {
  switch (d_type) {
    case VertexTypes::input:
      return "input";
    case VertexTypes::output:
      return "ouput";
    case VertexTypes::constant:
      return "const";
    case VertexTypes::gate:
      return "gate";
    case VertexTypes::graph:
      return "graph";
    default:
      return "";
  }
  return "";
}

void GraphVertexBase::setName(const std::string i_name) {
  d_name = i_name;
}

std::string GraphVertexBase::getName() const {
  return d_name;
}

void GraphVertexBase::setLevel(const unsigned i_level) {
  d_name = i_level;
}

unsigned GraphVertexBase::getLevel() const {
  return d_level;
}

void GraphVertexBase::updateLevel() {
  for (GraphVertexBase* vert : d_inConnections)
    d_level = (vert->getLevel() >= d_level) ? vert->getLevel() + 1 : d_level;
}

char GraphVertexBase::getValue() const {
  return d_value;
}

void GraphVertexBase::setBaseGraph(OrientedGraph* const i_baseGraph) {
  d_baseGraph = i_baseGraph;
}

OrientedGraph* GraphVertexBase::setBaseGraph() const {
  return d_baseGraph;
}

std::vector<GraphVertexBase*>& GraphVertexBase::getOutConnections() const {
    return d_outConnections;
}