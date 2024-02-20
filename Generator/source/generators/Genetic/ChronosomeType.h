#pragma once

#include <string>
#include <memory>

#include "settings/Settings.h"

/// class ChronosomeType
/// @param d_adaptationIndex This field represents the adaptation index for a particular chromosome. The adaptation index is often used in evolutionary algorithms to assess the quality or suitability of each chromosome in a population
/// @param d_chronosome This field stores the Type chromosome itself
/// @param d_setting A pointer to a settings object that can be used to set parameters or configurations for a given chromosome
/// @param d_name the name of the chromosome
/// 
/// 
/// */

template<typename Type, typename ParametersTypes>
class ChronosomeType
{
public:
  ChronosomeType(const std::string& i_name, const Type& i_chronosome);
  Type getChronosomeType() const;
  void setChronosomeType(const Type& i_chronosome);
  double getAdaptationIndex() const;
  void setAdaptationIndex(double i_adaptationIndex);
  std::string getName() const;
  void adaptationFunction();
  bool operator== (const ChronosomeType<Type, ParametersTypes>& r) const;

private:
  double d_adaptationIndex;
  Type d_chronosome;
  std::shared_ptr<Settings> d_settings = Settings::getInstance("GraphVertex");
  std::string d_name;
};
