#pragma once

enum SelectionTypes
{
  Base
};

/// class 
/// @param d_selectionType This field represents the type of selection of individuals (selection type) in a combinational scheme, which can be interpreted as one of the various types of selection algorithms used in genetic algorithms or other optimization methods
/// @param d_numOfSurvivors The number of input elements or states that pass through the circuit and remain "surviving" at the output
/// 
/// */

class SelectionParameters
{
public:
  int getNumOfSurvivors() const;
  void setNumOfSurvivors(int i_survivors);
  bool operator== (const SelectionParameters& r) const;
  void setSelectionType(SelectionTypes i_selectionType);
  SelectionTypes getSelectionType() const;
private:
  SelectionTypes d_selectionType = SelectionTypes::Base;
  int d_numOfSurvivors = 0;
};
