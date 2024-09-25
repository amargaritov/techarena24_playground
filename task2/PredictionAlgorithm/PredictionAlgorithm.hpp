/*
 * This file must declare the following structures and methods.
 * Please don't modify this file.
 */

#include <cstdint>

struct RoboPredictor {
  struct RoboMemory;
  RoboMemory* roboMemory_ptr;

  bool predictTimeOfDayOnNextPlanet(std::uint64_t nextPlanetID,
                                    bool spaceshipComuterPrediction,
                                    int nextPlanetGroupTag);

  void observeAndRecordTimeofdayOnNextPlanet(std::uint64_t nextPlanetID,
                                             bool timeOfDayOnNextPlanet);

  //---------------------------------------------------------------------
  // Since the struct RoboMemory is implicitly defined in .cpp
  // we need to move the declarations of constructor/destructor
  // to .cpp too
  RoboPredictor();
  ~RoboPredictor();
};
