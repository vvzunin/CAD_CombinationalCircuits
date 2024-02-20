#pragma once

enum ParentsTypes{
  Panmixia,
  Inbringing,
  Outbrinding,
  Tournament,
  Roulette
};

/// class ParentsParameters
/// @param d_tournamentNumber
/// @param Stores the number of participants in the tournament (tournament number) for the method of selecting parents using the tournament. This value indicates how many individuals will be selected from the population to participate in the tournament, from which the winner for the parent pair will be selected
/// @param d_parentsType Stores the parents type for the genetic algorithm. This enumeration (ParentsTypes) represents various methods of choosing parents, such as random selection, tournament selection, rating selection, etc.
/// 
/// */

class ParentsParameters {
public:
  void setParentsType(ParentsTypes i_parentsTypes);
  ParentsTypes getParentsType() const;
  void setTournamentNumber(int i_tournamentNumber);
  int getTournamentNumber() const;
  bool operator== (const ParentsParameters& r) const;

private:
  int d_tournamentNumber = 2;
  ParentsTypes d_parentsType = ParentsTypes::Panmixia;
};
