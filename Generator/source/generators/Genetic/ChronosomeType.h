#pragma once

#include <string>
#include <tuple>
#include <cmath>

#include "../../Settings.h"
#include "../TruthTable.h"


template<typename Type, typename ParametersTypes>
class ChronosomeType
{
public:
  ChronosomeType(const std::string & i_name, const Type & i_chronosome)
  {
    d_chronosome = i_chronosome;
    d_name = i_name;
    adaptationFunction();
  }
  ChronosomeType(const ChronosomeType & r)
    : d_chronosome(r.d_chronosome),
      d_name(r.d_name),
      d_adaptationIndex(r.d_adaptationIndex),
      d_settings(Settings::getInstance("TruthTable"))
  { }

  const Type & getChronosomeType() const
  {
    return d_chronosome;
  }
  void setChronosomeType(const Type& i_chronosome)
  {
    d_chronosome = i_chronosome;
  }
  double getAdaptationIndex() const
  {
    return d_adaptationIndex;
  }
  void setAdaptationIndex(double i_adaptationIndex)
  {
    d_adaptationIndex = i_adaptationIndex;
  }
  std::string getName() const
  {
    return d_name;
  }
  void adaptationFunction()
  {
    if constexpr (std::is_same_v<Type, TruthTable>)
    {
      int numInt = 0;
      for (const auto& nucleosoms : d_chronosome.getOutTable())
        for (const auto& nucleosom : nucleosoms)
          if (nucleosom)
            numInt++;
      d_adaptationIndex = abs(0.5 - (numInt / (d_chronosome.size() * d_chronosome.getOutput())));
    }
  }
  bool operator== (const ChronosomeType<Type, ParametersTypes>& r) const
  {
    return std::tie(d_adaptationIndex, d_chronosome, d_name) == std::tie(r.d_adaptationIndex, r.d_chronosome, r.d_name);
  }

private:
  double d_adaptationIndex;
  Type d_chronosome;
  Settings* d_settings = Settings::getInstance("GraphVertex");
  std::string d_name;
};

// template<typename Type, typename ParametersTypes>
// ChronosomeType<Type, ParametersTypes>::ChronosomeType(const std::string i_name, const Type i_chronosome)
// {
//   this->d_chronosome = i_chronosome;
//   this->d_name = i_name;
//   adaptationFunction();
// }

// template<typename Type, typename ParametersTypes>
// Type ChronosomeType<Type, ParametersTypes>::getChronosomeType() const
// {
//   return d_chronosome;
// }

// template<typename Type, typename ParametersTypes>
// void ChronosomeType<Type, ParametersTypes>::setChronosomeType(const Type& i_chronosome)
// {
//   d_chronosome = i_chronosome;
// }

// template<typename Type, typename ParametersTypes>
// void ChronosomeType<Type, ParametersTypes>::adaptationFunction()
// {
//   if constexpr (std::is_same_v<Type, TruthTable>)
//   {
//     int numInt = 0;
//     for (const auto& nucleosoms : d_chronosome.getOutTable())
//       for (const auto& nucleosom : nucleosoms)
//         if (nucleosom)
//           numInt++;
//     d_adaptationIndex = abs(0.5 - (numInt / (d_chronosome.size() * d_chronosome.getOutput())));
//   }
// }

// template<typename Type, typename ParametersTypes>
// double ChronosomeType<Type, ParametersTypes>::getAdaptationIndex() const
// {
//   return d_adaptationIndex;
// }

// template<typename Type, typename ParametersTypes>
// void ChronosomeType<Type, ParametersTypes>::setAdaptationIndex(double i_adaptationIndex)
// {
//   d_adaptationIndex = i_adaptationIndex;
// }

// template<typename Type, typename ParametersTypes>
// std::string ChronosomeType<Type, ParametersTypes>::getName() const
// {
//   return d_name;
// }

// template<typename Type, typename ParametersTypes>
// bool ChronosomeType<Type, ParametersTypes>::operator== (const ChronosomeType<Type, ParametersTypes>& r) const
// {
//   return std::tie(d_adaptationIndex, d_chronosome, d_name) == std::tie(r.d_adaptationIndex, r.d_chronosome, r.d_name);
// }